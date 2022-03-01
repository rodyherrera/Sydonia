/***
 * Copyright (C) Rodolfo Herrera Hernandez. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root 
 * for full license information.
 *
 * =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
 *
 * For related information - https://github.com/CodeWithRodi/Sydonia/
 *
 * =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 ****/

#ifndef SYDONIA_PARSER_HXX
#define SYDONIA_PARSER_HXX

#include <limits>
#include <stack>
#include <string>
#include <utility>
#include <vector>

#include "Configuration.hxx"
#include "Exceptions.hxx"
#include "FunctionStorage.hxx"
#include "Lexer.hxx"
#include "Node.hxx"
#include "Template.hxx"
#include "Token.hxx"
#include "Utilities.hxx"

namespace Sydonia{
    // ! Class for parsing a Sydonia Template
    class Parser{
        const ParserConfiguration &ParserConfigurationInstance;
        Lexer LexerInstance;
        TemplateStorage &TemplateStorageInstance;
        const FunctionStorage &FunctionStorageInstance;
        Token TokenInstance, PeekToken;
        bool HavePeekToken {false};
        size_t CurrentParenLevel {0};
        size_t CurrentBracketLevel {0};
        size_t CurrentBraceLevel {0};
        std::string_view LiteralStart;
        BlockNode* CurrentBlock {nullptr};
        ExpressionListNode* CurrentExpressionList {nullptr};
        std::stack<std::pair<FunctionNode*, size_t>> FunctionStack;
        std::vector<std::shared_ptr<ExpressionNode>> Arguments;
        std::stack<std::shared_ptr<FunctionNode>> OperatorStack;
        std::stack<IfStatementNode*> IfStatementStack;
        std::stack<ForStatementNode*> ForStatementStack;
        std::stack<BlockStatementNode*> BlockStatementStack;

        inline void ThrowParserError(const std::string &Message) const{
            SYDONIA_THROW(ParserError(Message, LexerInstance.CurrentPosition()));
        }

        inline void GetNextToken(){
            if(HavePeekToken){
                TokenInstance = PeekToken;
                HavePeekToken = false;
            }else
                TokenInstance = LexerInstance.Scan();
        }

        inline void GetPeekToken(){
            if(!HavePeekToken){
                PeekToken = LexerInstance.Scan();
                HavePeekToken = true;
            }
        }

        inline void AddLiteral(const char* ContentPointer){
            std::string_view DataText(LiteralStart.data(), TokenInstance.Text.data() - LiteralStart.data() + TokenInstance.Text.size());
            Arguments.emplace_back(std::make_shared<LiteralNode>(DataText, DataText.data() - ContentPointer));
        }

        inline void AddOperator(){
            auto Function = OperatorStack.top();
            OperatorStack.pop();
            for(int Iterator = 0; Iterator < Function->NumberArgs; ++Iterator){
                Function->Arguments.insert(Function->Arguments.begin(), Arguments.back());
                Arguments.pop_back();
            }
            Arguments.emplace_back(Function);
        }

        void AddToTemplateStorage(std::string_view Path, std::string &TemplateName){
            if(TemplateStorageInstance.find(TemplateName) != TemplateStorageInstance.end())
                return;
            std::string OriginalPath = static_cast<std::string>(Path);
            std::string OriginalName = TemplateName;
            if(ParserConfigurationInstance.SearchIncludedTemplatesInFiles){
                // ! Build the relative path
                TemplateName = OriginalPath + OriginalName;
                if(TemplateName.compare(0, 2, "./") == 0)
                    TemplateName.erase(0, 2);
                if(TemplateStorageInstance.find(TemplateName) == TemplateStorageInstance.end()){
                    // ! Load file
                    std::ifstream File;
                    File.open(TemplateName);
                    if(!File.fail()){
                        std::string Text((std::istreambuf_iterator<char>(File)), std::istreambuf_iterator<char>());
                        auto IncludeTemplate = Template(Text);
                        TemplateStorageInstance.emplace(TemplateName, IncludeTemplate);
                        ParseIntoTemplate(TemplateStorageInstance[TemplateName], TemplateName);
                        return;
                    }else if(!ParserConfigurationInstance.IncludeCallback)
                        SYDONIA_THROW(FileError("Failed accessing file at '" + TemplateName + "'"));
                }
            }

            // ! Try include callback
            if(ParserConfigurationInstance.IncludeCallback){
                auto IncludeTemplate = ParserConfigurationInstance.IncludeCallback(OriginalPath, OriginalName);
                TemplateStorageInstance.emplace(TemplateName, IncludeTemplate);
            }
        }

        std::string ParseFilename(const Token &TokenLocal) const{
            if(TokenLocal.KindInstance != Token::Kind::String)
                ThrowParserError("Expected string, got '" + TokenLocal.Describe() + "'");
            if(TokenLocal.Text.length() < 2)
                ThrowParserError("Expected filename, got '" + static_cast<std::string>(TokenLocal.Text) + "'");
            // ! Remove first and last character ""
            return std::string {TokenLocal.Text.substr(1, TokenLocal.Text.length() - 2)};
        }

        bool ParseExpression(Template &TemplateLocal, Token::Kind Closing){
            while(TokenInstance.KindInstance != Closing && TokenInstance.KindInstance != Token::Kind::Eof){
                // ! Literals
                switch(TokenInstance.KindInstance){
                    case Token::Kind::String: {
                        if(CurrentBraceLevel == 0 && CurrentBracketLevel == 0){
                            LiteralStart = TokenInstance.Text;
                            AddLiteral(TemplateLocal.Content.c_str());
                        }
                    } break;
                    case Token::Kind::Number: {
                        if(CurrentBraceLevel == 0 && CurrentBracketLevel == 0){
                            LiteralStart = TokenInstance.Text;
                            AddLiteral(TemplateLocal.Content.c_str());
                        }
                    } break;
                    case Token::Kind::LeftBracket: {
                        if(CurrentBraceLevel == 0 && CurrentBracketLevel == 0)
                            LiteralStart = TokenInstance.Text;
                        CurrentBracketLevel += 1;
                    } break;
                    case Token::Kind::LeftBrace: {
                        if(CurrentBraceLevel == 0 && CurrentBracketLevel == 0)
                            LiteralStart = TokenInstance.Text;
                        CurrentBraceLevel += 1;
                    } break;
                    case Token::Kind::RightBracket: {
                        if(CurrentBracketLevel == 0)
                            ThrowParserError("Unexpected ']'");
                        CurrentBracketLevel -= 1;
                        if(CurrentBraceLevel == 0 && CurrentBracketLevel == 0)
                            AddLiteral(TemplateLocal.Content.c_str());
                    } break;
                    case Token::Kind::RightBrace: {
                        if(CurrentBraceLevel == 0)
                            ThrowParserError("Unexpected '}'");
                        CurrentBraceLevel -= 1;
                        if(CurrentBraceLevel == 0 && CurrentBracketLevel == 0)
                            AddLiteral(TemplateLocal.Content.c_str());
                    } break;
                    case Token::Kind::Id: {
                        GetPeekToken();
                        // ! Data literal
                        if(TokenInstance.Text == static_cast<decltype(TokenInstance.Text)>("True") || TokenInstance.Text == static_cast<decltype(TokenInstance.Text)>("False") ||
                            TokenInstance.Text == static_cast<decltype(TokenInstance.Text)>("NULL")){
                                if(CurrentBraceLevel == 0 && CurrentBracketLevel == 0){
                                    LiteralStart = TokenInstance.Text;
                                    AddLiteral(TemplateLocal.Content.c_str());
                                }
                            }
                        // ! Operator
                        else if(TokenInstance.Text == "And" || TokenInstance.Text == "Or" || TokenInstance.Text == "In" || TokenInstance.Text == "Not")
                            goto ParseOperator;
                        // ! Functions
                        else if(PeekToken.KindInstance == Token::Kind::LeftParen){
                            OperatorStack.emplace(std::make_shared<FunctionNode>(static_cast<std::string>(TokenInstance.Text), TokenInstance.Text.data() - TemplateLocal.Content.c_str()));
                            FunctionStack.emplace(OperatorStack.top().get(), CurrentParenLevel);
                        // ! Variables
                        }else
                            Arguments.emplace_back(std::make_shared<DataNode>(static_cast<std::string>(TokenInstance.Text), TokenInstance.Text.data() - TemplateLocal.Content.c_str()));
                    // ! Operators
                    } break;
                    case Token::Kind::Equal:
                    case Token::Kind::NotEqual:
                    case Token::Kind::GreaterThan:
                    case Token::Kind::GreaterEqual:
                    case Token::Kind::LessThan:
                    case Token::Kind::LessEqual:
                    case Token::Kind::Plus:
                    case Token::Kind::Minus:
                    case Token::Kind::Times:
                    case Token::Kind::Slash:
                    case Token::Kind::Power:
                    case Token::Kind::Percent:
                    case Token::Kind::Dot: {
                        ParseOperator:
                            FunctionStorage::Operation OperationInstance;
                            switch(TokenInstance.KindInstance){
                                case Token::Kind::Id: {
                                    if(TokenInstance.Text == "And")
                                        OperationInstance = FunctionStorage::Operation::And;
                                    else if(TokenInstance.Text == "Or")
                                        OperationInstance = FunctionStorage::Operation::Or;
                                    else if(TokenInstance.Text == "In")
                                        OperationInstance = FunctionStorage::Operation::In;
                                    else if(TokenInstance.Text == "Not")
                                        OperationInstance = FunctionStorage::Operation::Not;
                                    else
                                        ThrowParserError("Unknown operator in parser.");
                                } break;
                                case Token::Kind::Equal: {
                                    OperationInstance = FunctionStorage::Operation::Equal;
                                } break;
                                case Token::Kind::NotEqual: {
                                    OperationInstance = FunctionStorage::Operation::NotEqual;
                                } break;
                                case Token::Kind::GreaterThan: {
                                    OperationInstance = FunctionStorage::Operation::Greater;
                                } break;
                                case Token::Kind::GreaterEqual: {
                                    OperationInstance = FunctionStorage::Operation::GreaterEqual;
                                } break;
                                case Token::Kind::LessThan: {
                                    OperationInstance = FunctionStorage::Operation::Less;
                                } break;
                                case Token::Kind::LessEqual: {
                                    OperationInstance = FunctionStorage::Operation::LessEqual;
                                } break;
                                case Token::Kind::Plus: {
                                    OperationInstance = FunctionStorage::Operation::Add;
                                } break;
                                case Token::Kind::Minus: {
                                    OperationInstance = FunctionStorage::Operation::Subtract;
                                } break;
                                case Token::Kind::Times: {
                                    OperationInstance = FunctionStorage::Operation::Multiplication;
                                } break;
                                case Token::Kind::Slash: {
                                    OperationInstance = FunctionStorage::Operation::Division;
                                } break;
                                case Token::Kind::Power: {
                                    OperationInstance = FunctionStorage::Operation::Power;
                                } break;
                                case Token::Kind::Percent: {
                                    OperationInstance = FunctionStorage::Operation::Modulo;
                                } break;
                                case Token::Kind::Dot: {
                                    OperationInstance = FunctionStorage::Operation::AtId;
                                } break;
                                default: {
                                    ThrowParserError("Unknown operator in parser.");
                                }
                            }
                            auto FunctionNodeInstance = std::make_shared<FunctionNode>(OperationInstance, TokenInstance.Text.data() - TemplateLocal.Content.c_str());
                            while(!OperatorStack.empty() &&
                                ((OperatorStack.top()->Precedence > FunctionNodeInstance->Precedence) ||
                                (OperatorStack.top()->Precedence == FunctionNodeInstance->Precedence && FunctionNodeInstance->AssociativityInstance == FunctionNode::Associativity::Left)) && 
                                (OperatorStack.top()->OperationInstance != FunctionStorage::Operation::ParenLeft)){
                                    AddOperator();
                                }
                            OperatorStack.emplace(FunctionNodeInstance);
                    } break;
                    case Token::Kind::Comma: {
                        if(CurrentBraceLevel == 0 && CurrentBracketLevel == 0){
                            if(FunctionStack.empty())
                                ThrowParserError("Unexpected ','");
                            FunctionStack.top().first->NumberArgs += 1;
                        }
                    } break;
                    case Token::Kind::Colon: {
                        if(CurrentBraceLevel == 0 && CurrentBracketLevel == 0)
                            ThrowParserError("Unexpected ':'");
                    } break;
                    case Token::Kind::LeftParen: {
                        CurrentParenLevel += 1;
                        OperatorStack.emplace(std::make_shared<FunctionNode>(FunctionStorage::Operation::ParenLeft, TokenInstance.Text.data() - TemplateLocal.Content.c_str()));
                        GetPeekToken();
                        if(PeekToken.KindInstance == Token::Kind::RightParen)
                            if(!FunctionStack.empty() && FunctionStack.top().second == CurrentParenLevel - 1)
                                FunctionStack.top().first->NumberArgs = 0;
                    } break;
                    case Token::Kind::RightParen: {
                        CurrentParenLevel -= 1;
                        while(!OperatorStack.empty() && OperatorStack.top()->OperationInstance != FunctionStorage::Operation::ParenLeft)
                            AddOperator();
                        if(!OperatorStack.empty() && OperatorStack.top()->OperationInstance == FunctionStorage::Operation::ParenLeft)
                            OperatorStack.pop();
                        if(!FunctionStack.empty() && FunctionStack.top().second == CurrentParenLevel){
                            auto Function = FunctionStack.top().first;
                            auto FunctionData = FunctionStorageInstance.FindFunction(Function->Name, Function->NumberArgs);
                            if(FunctionData.OperationInstance == FunctionStorage::Operation::None)
                                ThrowParserError("Unknown function " + Function->Name);
                            Function->OperationInstance = FunctionData.OperationInstance;
                            if(FunctionData.OperationInstance == FunctionStorage::Operation::Callback)
                                Function->Callback = FunctionData.Callback;
                            if(OperatorStack.empty())
                                ThrowParserError("Internal error at function " + Function->Name);
                            AddOperator();
                            FunctionStack.pop();
                        }
                    }
                    default:
                        break;
                }
                GetNextToken();
            }
            while(!OperatorStack.empty())
                AddOperator();
            if(Arguments.size() == 1){
                CurrentExpressionList->Root = Arguments[0];
                Arguments = {};
            }else if(Arguments.size() > 1)
                ThrowParserError("Malformed expression");
            return true;
        }

        bool ParseStatement(Template &TemplateLocal, Token::Kind Closing, std::string_view Path){
            if(TokenInstance.KindInstance != Token::Kind::Id)
                return false;
            if(TokenInstance.Text == static_cast<decltype(TokenInstance.Text)>("If")){
                GetNextToken();
                auto IfStatementNodeInstance = std::make_shared<IfStatementNode>(CurrentBlock, TokenInstance.Text.data() - TemplateLocal.Content.c_str());
                CurrentBlock->Nodes.emplace_back(IfStatementNodeInstance);
                IfStatementStack.emplace(IfStatementNodeInstance.get());
                CurrentBlock = &IfStatementNodeInstance->TrueStatement;
                CurrentExpressionList = &IfStatementNodeInstance->Condition;
                if(!ParseExpression(TemplateLocal, Closing))
                    return false;
            }else if(TokenInstance.Text == static_cast<decltype(TokenInstance.Text)>("Else")){
                if(IfStatementStack.empty())
                    ThrowParserError("Else without matching If");
                auto &IfStatementData = IfStatementStack.top();
                GetNextToken();
                IfStatementData->HasFalseStatement = true;
                CurrentBlock = &IfStatementData->FalseStatement;
                // ! Chained Else If
                if(TokenInstance.KindInstance == Token::Kind::Id && TokenInstance.Text == static_cast<decltype(TokenInstance.Text)>("If")){
                    GetNextToken();
                    auto IfStatementNodeInstance = std::make_shared<IfStatementNode>(true, CurrentBlock, TokenInstance.Text.data() - TemplateLocal.Content.c_str());
                    CurrentBlock->Nodes.emplace_back(IfStatementNodeInstance);
                    IfStatementStack.emplace(IfStatementNodeInstance.get());
                    CurrentBlock = &IfStatementNodeInstance->TrueStatement;
                    CurrentExpressionList = &IfStatementNodeInstance->Condition;
                    if(!ParseExpression(TemplateLocal, Closing))
                        return false;
                }
            }else if(TokenInstance.Text == static_cast<decltype(TokenInstance.Text)>("EndIf")){
                if(IfStatementStack.empty())
                    ThrowParserError("EndIf without matching If");
                // ! Nested if statements
                while(IfStatementStack.top()->IsNested)
                    IfStatementStack.pop();
                auto &IfStatementData = IfStatementStack.top();
                GetNextToken();
                CurrentBlock = IfStatementData->Parent;
                IfStatementStack.pop();
            }else if(TokenInstance.Text == static_cast<decltype(TokenInstance.Text)>("Block")){
                GetNextToken();
                if(TokenInstance.KindInstance != Token::Kind::Id)
                    ThrowParserError("Expected block name, got '" + TokenInstance.Describe() + "'");
                const std::string BlockName = static_cast<std::string>(TokenInstance.Text);
                auto BlockStatementNodeInstance = std::make_shared<BlockStatementNode>(CurrentBlock, BlockName, TokenInstance.Text.data() - TemplateLocal.Content.c_str());
                CurrentBlock->Nodes.emplace_back(BlockStatementNodeInstance);
                BlockStatementStack.emplace(BlockStatementNodeInstance.get());
                CurrentBlock = &BlockStatementNodeInstance->Block;
                auto Success = TemplateLocal.BlockStorage.emplace(BlockName, BlockStatementNodeInstance);
                if(!Success.second)
                    ThrowParserError("Block with the name '" + BlockName + "' does already exists");
                GetNextToken();
            }else if(TokenInstance.Text == static_cast<decltype(TokenInstance.Text)>("EndBlock")){
                if(BlockStatementStack.empty())
                    ThrowParserError("EndBlock without matching block");
                auto &BlockStatementData = BlockStatementStack.top();
                GetNextToken();
                CurrentBlock = BlockStatementData->Parent;
                BlockStatementStack.top();
            }else if(TokenInstance.Text == static_cast<decltype(TokenInstance.Text)>("For")){
                GetNextToken();
                // ! Options: For X, Y In Obj
                if(TokenInstance.KindInstance != Token::Kind::Id)
                    ThrowParserError("Expected id, got '" + TokenInstance.Describe() + "'");
                Token ValueToken = TokenInstance;
                GetNextToken();
                // ! Object type
                std::shared_ptr<ForStatementNode> ForStatementNodeInstance;
                if(TokenInstance.KindInstance == Token::Kind::Comma){
                    GetNextToken();
                    if(TokenInstance.KindInstance != Token::Kind::Id)
                        ThrowParserError("Expected id, got '" + TokenInstance.Describe() + "'");
                    Token KeyToken = std::move(ValueToken);
                    ValueToken = TokenInstance;
                    GetNextToken();
                    ForStatementNodeInstance = std::make_shared<ForObjectStatementNode>(static_cast<std::string>(KeyToken.Text), static_cast<std::string>(ValueToken.Text),
                                                                                        CurrentBlock, TokenInstance.Text.data() - TemplateLocal.Content.c_str());
                    // ! Array type
                }else{
                    ForStatementNodeInstance = std::make_shared<ForArrayStatementNode>(static_cast<std::string>(ValueToken.Text), CurrentBlock, TokenInstance.Text.data() - TemplateLocal.Content.c_str());
                    CurrentBlock->Nodes.emplace_back(ForStatementNodeInstance);
                    ForStatementStack.emplace(ForStatementNodeInstance.get());
                    CurrentBlock = &ForStatementNodeInstance->Body;
                    CurrentExpressionList = &ForStatementNodeInstance->Condition;
                    if(TokenInstance.KindInstance != Token::Kind::Id || TokenInstance.Text != static_cast<decltype(TokenInstance.Text)>("In"))
                        ThrowParserError("Expected 'In', got '" + TokenInstance.Describe() + "'");
                    GetNextToken();
                    if(!ParseExpression(TemplateLocal, Closing))
                        return false;
                }
            }else if(TokenInstance.Text == static_cast<decltype(TokenInstance.Text)>("EndFor")){
                if(ForStatementStack.empty())
                    ThrowParserError("EndFor without matching For");
                auto &ForStatementData = ForStatementStack.top();
                GetNextToken();
                CurrentBlock = ForStatementData->Parent;
                ForStatementStack.pop();
            }else if(TokenInstance.Text == static_cast<decltype(TokenInstance.Text)>("Include")){
                GetNextToken();
                std::string TemplateName = ParseFilename(TokenInstance);
                AddToTemplateStorage(Path, TemplateName);
                CurrentBlock->Nodes.emplace_back(std::make_shared<IncludeStatementNode>(TemplateName, TokenInstance.Text.data() - TemplateLocal.Content.c_str()));
                GetNextToken();
            }else if(TokenInstance.Text == static_cast<decltype(TokenInstance.Text)>("Extends")){
                GetNextToken();
                std::string TemplateName = ParseFilename(TokenInstance);
                AddToTemplateStorage(Path, TemplateName);
                CurrentBlock->Nodes.emplace_back(std::make_shared<ExtendsStatementNode>(TemplateName, TokenInstance.Text.data() - TemplateLocal.Content.c_str()));
                GetNextToken();
            }else if(TokenInstance.Text == static_cast<decltype(TokenInstance.Text)>("Set")){
                GetNextToken();
                if(TokenInstance.KindInstance != Token::Kind::Id)
                    ThrowParserError("Expected variable name, got '" + TokenInstance.Describe() + "'");
                std::string Key = static_cast<std::string>(TokenInstance.Text);
                GetNextToken();
                auto SetStatementNodeInstance = std::make_shared<SetStatementNode>(Key, TokenInstance.Text.data() - TemplateLocal.Content.c_str());
                CurrentBlock->Nodes.emplace_back(SetStatementNodeInstance);
                CurrentExpressionList = &SetStatementNodeInstance->Expression;
                if(TokenInstance.Text != static_cast<decltype(TokenInstance.Text)>("="))
                    ThrowParserError("Expected '=', got '" + TokenInstance.Describe() + "'");
                GetNextToken();
                if(!ParseExpression(TemplateLocal, Closing))
                    return false;
            }else
                return false;
            return true;
        }

        void ParseInto(Template &TemplateLocal, std::string_view Path){
            LexerInstance.Start(TemplateLocal.Content);
            CurrentBlock = &TemplateLocal.Root;
            for(;;){
                GetNextToken();
                switch(TokenInstance.KindInstance){
                    case Token::Kind::Eof: {
                        if(!IfStatementStack.empty())
                            ThrowParserError("Unmatched If");
                        if(!ForStatementStack.empty())
                            ThrowParserError("Unmatched For");
                    } return;
                    case Token::Kind::Text: {
                        CurrentBlock->Nodes.emplace_back(std::make_shared<TextNode>(TokenInstance.Text.data() - TemplateLocal.Content.c_str(), TokenInstance.Text.size()));
                    } break;
                    case Token::Kind::StatementOpen: {
                        GetNextToken();
                        if(!ParseStatement(TemplateLocal, Token::Kind::StatementClose, Path))
                            ThrowParserError("Expected statement, got '" + TokenInstance.Describe() + "'");
                        if(TokenInstance.KindInstance != Token::Kind::StatementClose)
                            ThrowParserError("Expected statement close, got '" + TokenInstance.Describe() + "'");
                    } break;
                    case Token::Kind::LineStatementOpen: {
                        GetNextToken();
                        if(!ParseStatement(TemplateLocal, Token::Kind::LineStatementClose, Path))
                            ThrowParserError("Expected statement, got '" + TokenInstance.Describe() + "'");
                        if(TokenInstance.KindInstance != Token::Kind::LineStatementClose && TokenInstance.KindInstance != Token::Kind::Eof)
                            ThrowParserError("Expected line statement close, got '" + TokenInstance.Describe() + "'");
                    } break;
                    case Token::Kind::ExpressionOpen: {
                        GetNextToken();
                        auto ExpressionListNodeInstance = std::make_shared<ExpressionListNode>(TokenInstance.Text.data() - TemplateLocal.Content.c_str());
                        CurrentBlock->Nodes.emplace_back(ExpressionListNodeInstance);
                        CurrentExpressionList = ExpressionListNodeInstance.get();
                        if(!ParseExpression(TemplateLocal, Token::Kind::ExpressionClose))
                            ThrowParserError("Expected expression, got '" + TokenInstance.Describe() + "'");
                        if(TokenInstance.KindInstance != Token::Kind::ExpressionClose)
                            ThrowParserError("Expected expression close, got '" + TokenInstance.Describe() + "'");
                    } break;
                    case Token::Kind::CommentOpen: {
                        GetNextToken();
                        if(TokenInstance.KindInstance != Token::Kind::CommentClose)
                            ThrowParserError("Expected comment close, got '" + TokenInstance.Describe() + "'");
                    } break;
                    default: {
                        ThrowParserError("Unexpected token '" + TokenInstance.Describe() + "'");
                    } break;
                }
            }
        }
        
        public:
            explicit Parser(const ParserConfiguration &ParserConfigurationLocal, const LexerConfiguration &LexerConfigurationLocal,
                            TemplateStorage &TemplateStorageLocal, const FunctionStorage &FunctionStorageLocal)
                : ParserConfigurationInstance(ParserConfigurationLocal), LexerInstance(LexerConfigurationLocal), TemplateStorageInstance(TemplateStorageLocal), FunctionStorageInstance(FunctionStorageLocal){}
            
            Template Parse(std::string_view Input, std::string_view Path){
                auto Result = Template(static_cast<std::string>(Input));
                ParseInto(Result, Path);
                return Result;
            }

            Template Parse(std::string_view Input){
                return Parse(Input, "./");
            }

            void ParseIntoTemplate(Template &TemplateLocal, std::string_view Filename){
                std::string_view Path = Filename.substr(0, Filename.find_last_of("/\\") + 1);
                auto SubParser = Parser(ParserConfigurationInstance, LexerInstance.GetConfiguration(), 
                                        TemplateStorageInstance, FunctionStorageInstance);
                SubParser.ParseInto(TemplateLocal, Path);
            }

            std::string LoadFile(const std::string &Filename){
                std::ifstream File;
                File.open(Filename);
                if(File.fail())
                    SYDONIA_THROW(FileError("Failed accessing file at '" + Filename + "'"));
                std::string Text((std::istreambuf_iterator<char>(File)), std::istreambuf_iterator<char>());
                return Text;
            }
    };
}; // ! Sydonia namespace

#endif // ! SYDONIA_PARSER_HXX