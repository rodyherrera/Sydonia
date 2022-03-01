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

#ifndef SYDONIA_HXX
#define SYDONIA_HXX

#include <nlohmann/json.hpp>

namespace Sydonia{
  using JSON = nlohmann::json;
}; // ! Namespace Sydonia

#if (defined(__cpp_exceptions) || defined(__EXCEPTIONS) || defined(_CPPUNWIND)) && !defined(SYDONIA_NOEXCEPTION)
  #ifndef SYDONIA_THROW
    #define SYDONIA_THROW(exception) throw exception
    #endif
  #else
    #include <cstdlib>
    #ifndef SYDONIA_THROW
      #define SYDONIA_THROW(exception)                                                                                                                                  \
        std::abort();                                                                                                                                                \
        std::ignore = exception
    #endif
    #ifndef SYDONIA_NOEXCEPTION
      #define SYDONIA_NOEXCEPTION
    #endif
#endif

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

#ifndef SYDONIA_ENVIRONMENT_HXX
#define SYDONIA_ENVIRONMENT_HXX

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <string_view>

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

#ifndef SYDONIA_CONFIGURATION_HXX
#define SYDONIA_CONFIGURATION_HXX

#include <functional>
#include <string>

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

#ifndef SYDONIA_TEMPLATE_HXX
#define SYDONIA_TEMPLATE_HXX

#include <map>
#include <memory>
#include <string>
#include <vector>

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

#ifndef SYDONIA_NODE_HXX
#define SYDONIA_NODE_HXX

#include <string>
#include <string_view>
#include <utility>

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

#ifndef SYDONIA_FUNCTION_STORAGE_HXX
#define SYDONIA_FUNCTION_STORAGE_HXX

#include <string_view>
#include <vector>

namespace Sydonia{
    using Arguments = std::vector<const JSON*>;
    using CallbackFunction = std::function<JSON(Arguments &LocalArguments)>;
    using VoidCallbackFunction = std::function<void(Arguments &LocalArguments)>;

    // ! Class for builtin functions and user-defined callbacks
    class FunctionStorage{
        public:
            enum class Operation{
                Not,
                And,
                Or,
                In,
                Equal,
                NotEqual,
                Greater,
                GreaterEqual,
                Less,
                LessEqual,
                Add,
                Subtract,
                Multiplication,
                Division,
                Power,
                Modulo,
                AtId,
                At,
                Default,
                DivisibleBy,
                Even,
                Exists,
                ExistsInObject,
                First,
                Float,
                Int,
                IsArray,
                IsBoolean,
                IsFloat,
                IsInteger,
                IsNumber,
                IsObject,
                IsString,
                Last,
                Length,
                Lower,
                Max,
                Min,
                Odd,
                Range,
                Round,
                Sort,
                Upper,
                Super,
                Join,
                Callback,
                ParenLeft,
                ParenRight,
                None,
            };

            struct FunctionData{
                explicit FunctionData(const Operation &OperationLocal, const CallbackFunction &CallbackLocal = CallbackFunction{})
                    : OperationInstance(OperationLocal), Callback(CallbackLocal){}
                const Operation OperationInstance;
                const CallbackFunction Callback;
            };
        private:
            const int Variadic {-1};

            std::map<std::pair<std::string, int>, FunctionData> FunctionStorage = {
                {std::make_pair("At", 2), FunctionData {Operation::At}},
                {std::make_pair("Default", 2), FunctionData {Operation::Default}},
                {std::make_pair("DivisibleBy", 2), FunctionData {Operation::DivisibleBy}},
                {std::make_pair("Even", 1), FunctionData {Operation::Even}},
                {std::make_pair("Exists", 1), FunctionData {Operation::Exists}},
                {std::make_pair("ExistsIn", 2), FunctionData {Operation::ExistsInObject}},
                {std::make_pair("First", 1), FunctionData {Operation::First}},
                {std::make_pair("Float", 1), FunctionData {Operation::Float}},
                {std::make_pair("Int", 1), FunctionData {Operation::Int}},
                {std::make_pair("IsArray", 1), FunctionData {Operation::IsArray}},
                {std::make_pair("IsBoolean", 1), FunctionData {Operation::IsBoolean}},
                {std::make_pair("IsFloat", 1), FunctionData {Operation::IsFloat}},
                {std::make_pair("IsInteger", 1), FunctionData {Operation::IsInteger}},
                {std::make_pair("IsNumber", 1), FunctionData {Operation::IsNumber}},
                {std::make_pair("IsObject", 1), FunctionData {Operation::IsObject}},
                {std::make_pair("IsString", 1), FunctionData {Operation::IsString}},
                {std::make_pair("Last", 1), FunctionData {Operation::Last}},
                {std::make_pair("Length", 1), FunctionData {Operation::Length}},
                {std::make_pair("Lower", 1), FunctionData {Operation::Lower}},
                {std::make_pair("Max", 1), FunctionData {Operation::Max}},
                {std::make_pair("Min", 1), FunctionData {Operation::Min}},
                {std::make_pair("Odd", 1), FunctionData {Operation::Odd}},
                {std::make_pair("Range", 1), FunctionData {Operation::Range}},
                {std::make_pair("Round", 2), FunctionData {Operation::Round}},
                {std::make_pair("Sort", 1), FunctionData {Operation::Sort}},
                {std::make_pair("Upper", 1), FunctionData {Operation::Upper}},
                {std::make_pair("Super", 0), FunctionData {Operation::Super}},
                {std::make_pair("Super", 1), FunctionData {Operation::Super}},
                {std::make_pair("Join", 2), FunctionData {Operation::Join}},  
            };

        public:
            void AddBuiltin(std::string_view Name, int NumArgs, Operation OperationLocal){
                FunctionStorage.emplace(std::make_pair(static_cast<std::string>(Name), NumArgs), FunctionData {OperationLocal});
            }

            void AddCallback(std::string_view Name, int NumArgs, const CallbackFunction &CallbackLocal){
                FunctionStorage.emplace(std::make_pair(static_cast<std::string>(Name), NumArgs), FunctionData {Operation::Callback, CallbackLocal});
            }

            FunctionData FindFunction(std::string_view Name, int NumArgs) const{
                auto Iterator = FunctionStorage.find(std::make_pair(static_cast<std::string>(Name), NumArgs));
                if(Iterator != FunctionStorage.end())
                    return Iterator->second;
                // ! Find variadic function
                else if(NumArgs > 0){
                    Iterator = FunctionStorage.find(std::make_pair(static_cast<std::string>(Name), Variadic));
                    if(Iterator != FunctionStorage.end())
                        return Iterator->second;
                }

                return FunctionData {Operation::None};
            };
    };
}; // ! Sydonia namespace

#endif // ! SYDONIA_FUNCTION_STORAGE_HXX
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

#ifndef SYDONIA_UTILITIES_HXX
#define SYDONIA_UTILITIES_HXX

#include <algorithm>
#include <fstream>
#include <string>
#include <string_view>
#include <utility>

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

#ifndef SYDONIA_EXCEPTIONS_HXX
#define SYDONIA_EXCEPTIONS_HXX

#include <stdexcept>
#include <string>

namespace Sydonia{
    struct SourceLocation{
        size_t Line;
        size_t Column;
    };

    struct SydoniaError : public std::runtime_error{
        const std::string Type;
        const std::string Message;
        const SourceLocation Location;

        explicit SydoniaError(const std::string &TypeLocal, const std::string &MessageLocal)
            : std::runtime_error("[Sydonia.Exception." + TypeLocal + "] " + MessageLocal), Type(TypeLocal), Message(MessageLocal), Location({0, 0}){}
        
        explicit SydoniaError(const std::string &TypeLocal, const std::string &MessageLocal, SourceLocation LocationLocal)
            : std::runtime_error("[Sydonia.Exception." + TypeLocal + "] (at " + std::to_string(LocationLocal.Line) + ":" + std::to_string(LocationLocal.Column) + ") " + MessageLocal),
                Type(TypeLocal), Message(MessageLocal), Location(LocationLocal){}
    };

    struct ParserError : public SydoniaError{
        explicit ParserError(const std::string &Message, SourceLocation Location): SydoniaError("ParserError", Message, Location){}
    };

    struct RenderError : public SydoniaError{
        explicit RenderError(const std::string &Message, SourceLocation Location): SydoniaError("RenderError", Message, Location){}
    };

    struct FileError : public SydoniaError{
        explicit FileError(const std::string &Message): SydoniaError("FileError", Message){}
        explicit FileError(const std::string &Message, SourceLocation Location): SydoniaError("FileError", Message, Location){}
    };

    struct DataError : public SydoniaError{
        explicit DataError(const std::string &Message, SourceLocation Location): SydoniaError("DataError", Message, Location){}
    };
}; // ! Sydonia namespace

#endif // ! SYDONIA_EXCEPTIONS_HXX

namespace Sydonia{
    namespace StringView{
        inline std::string_view Slice(std::string_view View, size_t Start, size_t End){
            Start = std::min(Start, View.size());
            End = std::min(std::max(Start, End), View.size());
            return View.substr(Start, End - Start);
        }

        inline std::pair<std::string_view, std::string_view> Split(std::string_view View, char Separator){
            size_t Idx = View.find(Separator);
            if(Idx == std::string::npos)
                return std::make_pair(View, std::string_view());
            return std::make_pair(Slice(View, 0, Idx), Slice(View, Idx + 1, std::string_view::npos));
        }

        inline bool StartsWith(std::string_view View, std::string_view Prefix){
            return (View.size() >= Prefix.size() && View.compare(0, Prefix.size(), Prefix) == 0);
        }
    }; // ! StringView namespace

    inline SourceLocation GetSourceLocation(std::string_view Content, size_t Position){
        // ! Get line and offset position (starts at 1:1)
        auto Sliced = StringView::Slice(Content, 0, Position);
        std::size_t LastNewLine = Sliced.rfind("\n");
        if(LastNewLine == std::string_view::npos)
            return {1, Sliced.length() + 1};
        // ! Count newlines
        size_t CountLines = 0;
        size_t SearchStart = 0;
        while(SearchStart <= Sliced.size()){
            SearchStart = Sliced.find("\n", SearchStart) + 1;
            if(SearchStart == 0)
                break;
            CountLines += 1;
        }
        return {CountLines + 1, Sliced.length() - LastNewLine};
    }

    inline void ReplaceSubString(std::string &S, const std::string &F, const std::string &T){
        if(F.empty())
            return;
        for(auto Position = S.find(F); // ! Find first ocurrence of F
            Position != std::string::npos; // ! Make sure F was found
            S.replace(Position, F.size(), T), // ! Replace with T, and 
            Position = S.find(F, Position + T.size())){}  // ! Find next ocurrence of F
    }
};  // ! Sydonia namespace

#endif // ! SYDONIA_UTILTIES_HXX

namespace Sydonia{
    class NodeVisitor;
    class BlockNode;
    class TextNode;
    class ExpressionNode;
    class LiteralNode;
    class DataNode;
    class FunctionNode;
    class ExpressionListNode;
    class StatementNode;
    class ForStatementNode;
    class ForArrayStatementNode;
    class ForObjectStatementNode;
    class IfStatementNode;
    class IncludeStatementNode;
    class ExtendsStatementNode;
    class BlockStatementNode;
    class SetStatementNode;

    class NodeVisitor{
        public:
            virtual ~NodeVisitor() = default;
            
            virtual void Visit(const BlockNode& Node) = 0;
            virtual void Visit(const TextNode& Node) = 0;
            virtual void Visit(const ExpressionNode& Node) = 0;
            virtual void Visit(const LiteralNode& Node) = 0;
            virtual void Visit(const DataNode& Node) = 0;
            virtual void Visit(const FunctionNode& Node) = 0;
            virtual void Visit(const ExpressionListNode& Node) = 0;
            virtual void Visit(const StatementNode& Node) = 0;
            virtual void Visit(const ForStatementNode& Node) = 0;
            virtual void Visit(const ForArrayStatementNode& Node) = 0;
            virtual void Visit(const ForObjectStatementNode& Node) = 0;
            virtual void Visit(const IfStatementNode& Node) = 0;
            virtual void Visit(const IncludeStatementNode& Node) = 0;
            virtual void Visit(const ExtendsStatementNode& Node) = 0;
            virtual void Visit(const BlockStatementNode& Node) = 0;
            virtual void Visit(const SetStatementNode& Node) = 0;
    };

    // ! Base node class for the abstract syntax tree (AST)
    class AstNode{
        public:
            virtual void Accept(NodeVisitor &Visitor) const = 0;
            size_t Position;
            AstNode(size_t PositionLocal): Position(PositionLocal){}
            virtual ~AstNode(){}
    };

    class BlockNode : public AstNode{
        public:
            std::vector<std::shared_ptr<AstNode>> Nodes;
            explicit BlockNode(): AstNode(0){}
            void Accept(NodeVisitor &Visitor) const{
                Visitor.Visit(*this);
            }
    };

    class TextNode : public AstNode{
        public:
            const size_t Length;
            explicit TextNode(size_t Position, size_t LengthLocal): AstNode(Position), Length(LengthLocal){}
            void Accept(NodeVisitor &Visitor) const{
                Visitor.Visit(*this);
            }
    };

    class ExpressionNode : public AstNode{
        public:
            explicit ExpressionNode(size_t Position): AstNode(Position){}
            void Accept(NodeVisitor &Visitor) const{
                Visitor.Visit(*this);
            }
    };

    class LiteralNode : public ExpressionNode{
        public:
            const JSON Value;
            explicit LiteralNode(std::string_view DataText, size_t Position): ExpressionNode(Position), Value(JSON::parse(DataText)){}
            void Accept(NodeVisitor &Visitor) const{
                Visitor.Visit(*this);
            }
    };

    class DataNode : public ExpressionNode{
        public:
            const std::string Name;
            const JSON::json_pointer Pointer;

            static std::string ConvertDotToPointer(std::string_view PointerName){
                std::string Result;
                do{
                    std::string_view Part;
                    std::tie(Part, PointerName) = StringView::Split(PointerName, '.');
                    Result.push_back('/');
                    Result.append(Part.begin(), Part.end());
                }while(!PointerName.empty());
                return Result;
            }

            explicit DataNode(std::string_view PointerNameLocal, size_t Position):  ExpressionNode(Position), Name(PointerNameLocal), Pointer(JSON::json_pointer(ConvertDotToPointer(PointerNameLocal))){}

            void Accept(NodeVisitor &Visitor) const{
                Visitor.Visit(*this);
            }
    };

    class FunctionNode : public ExpressionNode{
        using Operation = FunctionStorage::Operation;

        public:
            enum class Associativity{
                Left,
                Right
            };

            unsigned int Precedence;
            Associativity AssociativityInstance;

            Operation OperationInstance;

            std::string Name;
            // ! Should also be negative -> -1 for unknown number
            int NumberArgs;
            std::vector<std::shared_ptr<ExpressionNode>> Arguments;
            CallbackFunction Callback;

            explicit FunctionNode(std::string_view NameLocal, size_t Position)
                : ExpressionNode(Position), Precedence(8), AssociativityInstance(Associativity::Left), OperationInstance(Operation::Callback), Name(NameLocal), NumberArgs(1){} 
            
            explicit FunctionNode(Operation OperationLocal, size_t Position): ExpressionNode(Position), OperationInstance(OperationLocal), NumberArgs(1){
                switch(OperationLocal){
                    case Operation::Not: {
                        NumberArgs = 1;
                        Precedence = 4;
                        AssociativityInstance = Associativity::Left;
                    } break;
                    case Operation::And: {
                        NumberArgs = 2;
                        Precedence = 1;
                        AssociativityInstance = Associativity::Left;
                    } break;
                    case Operation::Or: {
                        NumberArgs = 2;
                        Precedence = 1;
                        AssociativityInstance = Associativity::Left;
                    } break;
                    case Operation::In: {
                        NumberArgs = 2;
                        Precedence = 2;
                        AssociativityInstance = Associativity::Left;
                    } break;
                    case Operation::Equal: {
                        NumberArgs = 2;
                        Precedence = 2;
                        AssociativityInstance = Associativity::Left;
                    } break;
                    case Operation::NotEqual: {
                        NumberArgs = 2;
                        Precedence = 2;
                        AssociativityInstance = Associativity::Left;
                    } break;
                    case Operation::Greater: {
                        NumberArgs = 2;
                        Precedence = 2;
                        AssociativityInstance = Associativity::Left;
                    } break;
                    case Operation::GreaterEqual: {
                        NumberArgs = 2;
                        Precedence = 2;
                        AssociativityInstance = Associativity::Left;
                    } break;
                    case Operation::Less: {
                        NumberArgs = 2;
                        Precedence = 2;
                        AssociativityInstance = Associativity::Left;
                    } break;
                    case Operation::LessEqual: {
                        NumberArgs = 2;
                        Precedence = 2;
                        AssociativityInstance = Associativity::Left;
                    } break;
                    case Operation::Add: {
                        NumberArgs = 2;
                        Precedence = 3;
                        AssociativityInstance = Associativity::Left;
                    } break;
                    case Operation::Subtract: {
                        NumberArgs = 2;
                        Precedence = 3;
                        AssociativityInstance = Associativity::Left;
                    } break;
                    case Operation::Multiplication: {
                        NumberArgs = 2;
                        Precedence = 4;
                        AssociativityInstance = Associativity::Left;
                    } break;
                    case Operation::Division: {
                        NumberArgs = 2;
                        Precedence = 4;
                        AssociativityInstance = Associativity::Left;
                    } break;
                    case Operation::Power: {
                        NumberArgs = 2;
                        Precedence = 5;
                        AssociativityInstance = Associativity::Right;
                    } break;
                    case Operation::Modulo: {
                        NumberArgs = 2;
                        Precedence = 4;
                        AssociativityInstance = Associativity::Left;
                    } break;
                    case Operation::AtId: {
                        NumberArgs = 2;
                        Precedence = 8;
                        AssociativityInstance = Associativity::Left;
                    } break;
                    default: {
                        Precedence = 1;
                        AssociativityInstance = Associativity::Left;
                    }
                }
            }

            void Accept(NodeVisitor &Visitor) const{
                Visitor.Visit(*this);
            }
    };

    class ExpressionListNode : public AstNode{
        public:
            std::shared_ptr<ExpressionNode> Root;
            explicit ExpressionListNode(): AstNode(0){}
            explicit ExpressionListNode(size_t Position): AstNode(Position){}

            void Accept(NodeVisitor &Visitor) const{
                Visitor.Visit(*this);
            }
    };

    class StatementNode : public AstNode{
        public:
            StatementNode(size_t Position): AstNode(Position){}
            virtual void Accept(NodeVisitor &Visitor) const = 0;
    };

    class ForStatementNode : public StatementNode{
        public:
            ExpressionListNode Condition;
            BlockNode Body;
            BlockNode* const Parent;
            
            ForStatementNode(BlockNode* const ParentLocal, size_t Position): StatementNode(Position), Parent(ParentLocal){}

            virtual void Accept(NodeVisitor &Visitor) const = 0;
    };

    class ForArrayStatementNode : public ForStatementNode{
        public:
            const std::string Value;

            explicit ForArrayStatementNode(const std::string &ValueLocal, BlockNode* const Parent, size_t Position): ForStatementNode(Parent, Position), Value(ValueLocal){}

            void Accept(NodeVisitor &Visitor) const{
                Visitor.Visit(*this);
            }
    };

    class ForObjectStatementNode : public ForStatementNode{
        public:
            const std::string Key;
            const std::string Value;

            explicit ForObjectStatementNode(const std::string &KeyLocal, const std::string &ValueLocal, BlockNode* const Parent, size_t Position)
                : ForStatementNode(Parent, Position), Key(KeyLocal), Value(ValueLocal){}
            
            void Accept(NodeVisitor &Visitor) const{
                Visitor.Visit(*this);
            }
    };

    class IfStatementNode : public StatementNode{
        public:
            ExpressionListNode Condition;
            BlockNode TrueStatement;
            BlockNode FalseStatement;
            BlockNode* const Parent;

            const bool IsNested;
            bool HasFalseStatement {false};

            explicit IfStatementNode(BlockNode* const ParentLocal, size_t Position): StatementNode(Position), Parent(ParentLocal), IsNested(false){}
            explicit IfStatementNode(bool IsNestedLocal, BlockNode* const ParentLocal, size_t Position): StatementNode(Position), Parent(ParentLocal), IsNested(IsNestedLocal){}

            void Accept(NodeVisitor &Visitor) const{
                Visitor.Visit(*this);
            }
    };

    class IncludeStatementNode : public StatementNode{
        public:
            const std::string File;

            explicit IncludeStatementNode(const std::string &FileLocal, size_t Position): StatementNode(Position), File(FileLocal){}

            void Accept(NodeVisitor &Visitor) const{
                Visitor.Visit(*this);
            }
    };

    class ExtendsStatementNode : public StatementNode{
        public:
            const std::string File;
            
            explicit ExtendsStatementNode(const std::string &FileLocal, size_t Position): StatementNode(Position), File(FileLocal){}

            void Accept(NodeVisitor &Visitor) const{
                Visitor.Visit(*this);
            }
    };

    class BlockStatementNode : public StatementNode{
        public:
            const std::string Name;
            BlockNode Block;
            BlockNode* const Parent;

            explicit BlockStatementNode(BlockNode* const ParentLocal, const std::string &NameLocal, size_t Position): StatementNode(Position), Name(NameLocal), Parent(ParentLocal){} 

            void Accept(NodeVisitor &Visitor) const{
                Visitor.Visit(*this);
            }
    };

    class SetStatementNode : public StatementNode{
        public:
            const std::string Key;
            ExpressionListNode Expression;

            explicit SetStatementNode(const std::string &KeyLocal, size_t Position): StatementNode(Position), Key(KeyLocal){}

            void Accept(NodeVisitor &Visitor) const{
                Visitor.Visit(*this);
            }
    };
}; // ! Sydonia namespace

#endif // ! SYDONIA_NODE_HXX
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

#ifndef SYDONIA_STATISTICS_HXX
#define SYDONIA_STATISTICS_HXX

namespace Sydonia{
    // ! A class for counting statistics on a template
    class StatisticsVisitor : public NodeVisitor{
        void Visit(const BlockNode &Node){
            for(auto &SubNode : Node.Nodes)
                SubNode->Accept(*this);
        }
        
        void Visit(const TextNode&){}
        void Visit(const ExpressionNode&){}
        void Visit(const LiteralNode&){}

        void Visit(const DataNode&){
            VariableCounter += 1;
        }

        void Visit(const FunctionNode &Node){
            for(auto &SubNode : Node.Arguments)
                SubNode->Accept(*this);
        }

        void Visit(const ExpressionListNode &Node){
            Node.Root->Accept(*this);
        }

        void Visit(const StatementNode&){}
        void Visit(const ForStatementNode&){}
        
        void Visit(const ForArrayStatementNode &Node){
            Node.Condition.Accept(*this);
            Node.Body.Accept(*this);
        }

        void Visit(const ForObjectStatementNode &Node){
            Node.Condition.Accept(*this);
            Node.Body.Accept(*this);
        }

        void Visit(const IfStatementNode &Node){
            Node.Condition.Accept(*this);
            Node.TrueStatement.Accept(*this);
            Node.FalseStatement.Accept(*this);
        }

        void Visit(const IncludeStatementNode&){}

        void Visit(const ExtendsStatementNode&){}

        void Visit(const BlockStatementNode &Node){
            Node.Block.Accept(*this);
        }

        void Visit(const SetStatementNode&){}

        public:
            unsigned VariableCounter;
            explicit StatisticsVisitor(): VariableCounter(0){}
    };
}; // ! Sydonia namespace

#endif // ! SYDONIA_STATISTICS_HXX

namespace Sydonia{
    // ! The main Sydonia Template
    struct Template{
        BlockNode Root;
        std::string Content;
        std::map<std::string, std::shared_ptr<BlockStatementNode>> BlockStorage;
        
        explicit Template(){}
        explicit Template(const std::string &ContentLocal): Content(ContentLocal){}

        // ! Return number of variables (total number, not distinct ones) in the template
        int CountVariables(){
            auto StatisticVisitor = StatisticsVisitor();
            Root.Accept(StatisticVisitor);
            return StatisticVisitor.VariableCounter;
        }
    };

    using TemplateStorage = std::map<std::string, Template>;
}; // ! Sydonia namespace

#endif // ! SYDONIA_TEMPLATE_HXX

namespace Sydonia{
    // ! Struct for lexer configuration
    struct LexerConfiguration{
        std::string StatementOpen {"{%"};
        std::string StatementOpenNoLstrip {"{%+"};
        std::string StatementOpenForceLstrip {"{%-"};
        std::string StatementClose {"%}"};
        std::string StatementCloseForceRstrip {"-%}"};
        std::string LineStatement {"##"};
        std::string ExpressionOpen {"{{"};
        std::string ExpressionOpenForceLstrip {"{{-"};
        std::string ExpressionClose {"}}"};
        std::string ExpressionCloseForceRstrip {"-}}"};
        std::string CommentOpen {"{#"};
        std::string CommentOpenForceLstrip {"{#-"};
        std::string CommentClose {"#}"};
        std::string CommentCloseForceRstrip {"-#}"};
        std::string OpenCharacters {"#{"};

        bool TrimBlocks {false};
        bool LstripBlocks {false};

        void UpdateOpenCharacters(){
            OpenCharacters = "";
            if(OpenCharacters.find(LineStatement[0]) == std::string::npos)
                OpenCharacters += LineStatement[0];
            if(OpenCharacters.find(StatementOpen[0]) == std::string::npos)
                OpenCharacters += StatementOpen[0];
            if(OpenCharacters.find(StatementOpenNoLstrip[0]) == std::string::npos)
                OpenCharacters += StatementOpenNoLstrip[0];
            if(OpenCharacters.find(StatementOpenForceLstrip[0]) == std::string::npos)
                OpenCharacters += StatementOpenForceLstrip[0];
            if(OpenCharacters.find(ExpressionOpen[0]) == std::string::npos)
                OpenCharacters += ExpressionOpen[0];
            if(OpenCharacters.find(ExpressionOpenForceLstrip[0]) == std::string::npos)
                OpenCharacters += ExpressionOpenForceLstrip[0];
            if(OpenCharacters.find(CommentOpen[0]) == std::string::npos)
                OpenCharacters += CommentOpen[0];
            if(OpenCharacters.find(CommentOpenForceLstrip[0]) == std::string::npos)
                OpenCharacters += CommentOpenForceLstrip[0];
        }
    };

    // ! Struct for parser configuration
    struct ParserConfiguration{
        bool SearchIncludedTemplatesInFiles {true};
        std::function<Template(const std::string &, const std::string &)> IncludeCallback;
    };

    // ! Struct for render configuration
    struct RenderConfiguration{
        bool ThrowAtMissingIncludes {true};
    };
}; // ! Sydonia namespace

#endif // ! SYDONIA_CONFIGURATION_HXX

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

#ifndef SYDONIA_LEXER_HXX
#define SYDONIA_LEXER_HXX

#include <cctype>
#include <locale>

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

#ifndef SYDONIA_TOKEN_HXX
#define SYDONIA_TOKEN_HXX

#include <string>
#include <string_view>

namespace Sydonia{
    // ! Helper struct for the Sydonia Lexer
    struct Token{
        enum class Kind{
            Text,
            ExpressionOpen,     // {{
            ExpressionClose,    // }}
            LineStatementOpen,  // ##
            LineStatementClose, // \n
            StatementOpen,      // {%
            StatementClose,     // %}
            CommentOpen,        // {#
            CommentClose,       // #}
            Id,                 // this, this.foo
            Number,             // 1, 2, -1, 5.2, -5.3
            String,             // "this"
            Plus,               // +
            Minus,              // -
            Times,              // *
            Slash,              // /
            Percent,            // %
            Power,              // ^
            Comma,              // ,
            Dot,                // .
            Colon,              // :
            LeftParen,          // (
            RightParen,         // )
            LeftBracket,        // [
            RightBracket,       // ]
            LeftBrace,          // {
            RightBrace,         // }
            Equal,              // ==
            NotEqual,           // !=
            GreaterThan,        // >
            GreaterEqual,       // >=
            LessThan,           // <
            LessEqual,          // <=
            Unknown,
            Eof,
        };

        Kind KindInstance {Kind::Unknown};
        std::string_view Text;

        explicit constexpr Token() = default;
        explicit constexpr Token(Kind KindLocalInstance, std::string_view TextLocal)
            : KindInstance(KindLocalInstance), Text(TextLocal){}

        std::string Describe() const{
            switch(KindInstance){
                case Kind::Text:
                    return "<Text>";
                case Kind::LineStatementClose:
                    return "<Eol>";
                case Kind::Eof:
                    return "<Eof>";
                default:
                    return static_cast<std::string>(Text);
            }
        } 
    };
}; // ! Sydonia namespace

#endif // ! SYDONIA_TOKEN_HXX

namespace Sydonia{
  // ! Class for lexing an Sydonia Template.
  class Lexer{
    enum class State{
      Text,
      ExpressionStart,
      ExpressionStartForceLstrip,
      ExpressionBody,
      LineStart,
      LineBody,
      StatementStart,
      StatementStartNoLstrip,
      StatementStartForceLstrip,
      StatementBody,
      CommentStart,
      CommentStartForceLstrip,
      CommentBody,
    };

    enum class MinusState{
      Operator,
      Number,
    };

    const LexerConfiguration& LexerConfigurationInstance;

    State StateInstance;
    MinusState MinusStateInstance;
    std::string_view M_IN;
    size_t TokenStart;
    size_t Position;

    Token ScanBody(std::string_view Close, Token::Kind CloseKind, std::string_view CloseTrim = std::string_view(), bool Trim = false){
    Again:
      // ! Skip whitespace (except for \n as it might be a close)
      if(TokenStart >= M_IN.size())
        return MakeToken(Token::Kind::Eof);
      const char Character = M_IN[TokenStart];
      if(Character == ' ' || Character == '\t' || Character == '\r') {
        TokenStart += 1;
        goto Again;
      }

      // ! Check for close
      if(!CloseTrim.empty() && StringView::StartsWith(M_IN.substr(TokenStart), CloseTrim)){
        StateInstance = State::Text;
        Position = TokenStart + CloseTrim.size();
        const Token TokenInstance = MakeToken(CloseKind);
        SkipWhiteSpacesAndNewLines();
        return TokenInstance;
      }

      if(StringView::StartsWith(M_IN.substr(TokenStart), Close)){
        StateInstance = State::Text;
        Position = TokenStart + Close.size();
        const Token TokenInstance = MakeToken(CloseKind);
        if(Trim)
          SkipWhiteSpacesAndFirstNewLine();
        return TokenInstance;
      }

      // ! Skip \n
      if(Character == '\n'){
        TokenStart += 1;
        goto Again;
      }

      Position = TokenStart + 1;
      if(std::isalpha(Character)){
        MinusStateInstance = MinusState::Operator;
        return ScanID();
      }

      const MinusState CurrentMinusState = MinusStateInstance;
      if(MinusStateInstance == MinusState::Operator)
        MinusStateInstance = MinusState::Number;

      switch(Character){
        case '+':
          return MakeToken(Token::Kind::Plus);
        case '-':
          if(CurrentMinusState == MinusState::Operator)
            return MakeToken(Token::Kind::Minus);
          return ScanNumber();
        case '*':
          return MakeToken(Token::Kind::Times);
        case '/':
          return MakeToken(Token::Kind::Slash);
        case '^':
          return MakeToken(Token::Kind::Power);
        case '%':
          return MakeToken(Token::Kind::Percent);
        case '.':
          return MakeToken(Token::Kind::Dot);
        case ',':
          return MakeToken(Token::Kind::Comma);
        case ':':
          return MakeToken(Token::Kind::Colon);
        case '(':
          return MakeToken(Token::Kind::LeftParen);
        case ')':
          MinusStateInstance = MinusState::Operator;
          return MakeToken(Token::Kind::RightParen);
        case '[':
          return MakeToken(Token::Kind::LeftBracket);
        case ']':
          MinusStateInstance = MinusState::Operator;
          return MakeToken(Token::Kind::RightBracket);
        case '{':
          return MakeToken(Token::Kind::LeftBrace);
        case '}':
          MinusStateInstance = MinusState::Operator;
          return MakeToken(Token::Kind::RightBrace);
        case '>':
          if(Position < M_IN.size() && M_IN[Position] == '='){
            Position += 1;
            return MakeToken(Token::Kind::GreaterEqual);
          }
          return MakeToken(Token::Kind::GreaterThan);
        case '<':
          if (Position < M_IN.size() && M_IN[Position] == '='){
            Position += 1;
            return MakeToken(Token::Kind::LessEqual);
          }
          return MakeToken(Token::Kind::LessThan);
        case '=':
          if(Position < M_IN.size() && M_IN[Position] == '='){
            Position += 1;
            return MakeToken(Token::Kind::Equal);
          }
          return MakeToken(Token::Kind::Unknown);
        case '!':
          if(Position < M_IN.size() && M_IN[Position] == '='){
            Position += 1;
            return MakeToken(Token::Kind::NotEqual);
          }
          return MakeToken(Token::Kind::Unknown);
        case '\"':
          return ScanString();
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
          MinusStateInstance = MinusState::Operator;
          return ScanNumber();
        case '_':
        case '@':
        case '$':
          MinusStateInstance = MinusState::Operator;
          return ScanID();
        default:
          return MakeToken(Token::Kind::Unknown);
      }
    }

    Token ScanID(){
      for(;;){
        if(Position >= M_IN.size())
          break;
        const char Character = M_IN[Position];
        if(!std::isalnum(Character) && Character != '.' && Character != '/' && Character != '_' && Character != '-')
          break;
        Position += 1;
      }
      return MakeToken(Token::Kind::Id);
    }

    Token ScanNumber(){
      for(;;){
        if(Position >= M_IN.size())
          break;
        const char Character = M_IN[Position];
        // ! Be very permissive in lexer (we'll catch errors when conversion happens)
        if(!(std::isdigit(Character) || Character == '.' || Character == 'e' || Character == 'E' || (Character == '+' && (Position == 0 || M_IN[Position - 1] == 'e' || M_IN[Position - 1] == 'E')) || (Character == '-' && (Position == 0 || M_IN[Position - 1] == 'e' || M_IN[Position - 1] == 'E'))))
          break;
        Position += 1;
      }
      return MakeToken(Token::Kind::Number);
    }

    Token ScanString(){
      bool Escape {false};
      for(;;){
        if(Position >= M_IN.size())
          break;
        const char Character = M_IN[Position++];
        if(Character == '\\')
          Escape = true;
        else if(!Escape && Character == M_IN[TokenStart])
          break;
        else
          Escape = false;
      }
      return MakeToken(Token::Kind::String);
    }

    Token MakeToken(Token::Kind KindLocal) const{
      return Token(KindLocal, StringView::Slice(M_IN, TokenStart, Position));
    }

    void SkipWhiteSpacesAndNewLines(){
      if(Position < M_IN.size())
        while(Position < M_IN.size() && (M_IN[Position] == ' ' || M_IN[Position] == '\t' || M_IN[Position] == '\n' || M_IN[Position] == '\r'))
          Position += 1;
    }

    void SkipWhiteSpacesAndFirstNewLine() {
      if(Position < M_IN.size())
        while(Position < M_IN.size() && (M_IN[Position] == ' ' || M_IN[Position] == '\t'))
          Position += 1;

      if(Position < M_IN.size()){
        const char Character = M_IN[Position];
        if(Character == '\n')
          Position += 1;
        else if(Character == '\r'){
          Position += 1;
          if(Position < M_IN.size() && M_IN[Position] == '\n')
            Position += 1;
        }
      }
    }

    static std::string_view ClearFinalLineIfWhiteSpace(std::string_view Text){
      std::string_view Result = Text;
      while(!Result.empty()){
        const char Character = Result.back();
        if (Character == ' ' || Character == '\t')
          Result.remove_suffix(1);
        else if (Character == '\n' || Character == '\r')
          break;
        else
          return Text;
      }
      return Result;
    }

  public:
    explicit Lexer(const LexerConfiguration& LexerConfigurationLocal)
      : LexerConfigurationInstance(LexerConfigurationLocal), StateInstance(State::Text), 
        MinusStateInstance(MinusState::Number){}

    SourceLocation CurrentPosition() const{
      return GetSourceLocation(M_IN, TokenStart);
    }

    void Start(std::string_view Input){
      M_IN = Input;
      TokenStart = 0;
      Position = 0;
      StateInstance = State::Text;
      MinusStateInstance = MinusState::Number;

      // ! Consume byte order mark (BOM) for UTF-8
      if(StringView::StartsWith(M_IN, "\xEF\xBB\xBF"))
        M_IN = M_IN.substr(3);
    }

    Token Scan(){
      TokenStart = Position;

    Again:
      if(TokenStart >= M_IN.size())
        return MakeToken(Token::Kind::Eof);

      switch(StateInstance){
        default:
        case State::Text: {
          // ! Fast-scan to first open character
          const size_t OpenStart = M_IN.substr(Position).find_first_of(LexerConfigurationInstance.OpenCharacters);
          if(OpenStart == std::string_view::npos){
            // ! Didn't find open, return remaining text as text token
            Position = M_IN.size();
            return MakeToken(Token::Kind::Text);
          }
          Position += OpenStart;

          // ! Try to match one of the opening sequences, and get the close
          std::string_view OpenString = M_IN.substr(Position);
          bool MustLstrip = false;
          if(StringView::StartsWith(OpenString, LexerConfigurationInstance.ExpressionOpen)){
            if(StringView::StartsWith(OpenString, LexerConfigurationInstance.ExpressionOpenForceLstrip)){
              StateInstance = State::ExpressionStartForceLstrip;
              MustLstrip = true;
            }else
              StateInstance = State::ExpressionStart;
          }else if(StringView::StartsWith(OpenString, LexerConfigurationInstance.StatementOpen)){
            if(StringView::StartsWith(OpenString, LexerConfigurationInstance.StatementOpenNoLstrip))
              StateInstance = State::StatementStartNoLstrip;
            else if(StringView::StartsWith(OpenString, LexerConfigurationInstance.StatementOpenForceLstrip)){
              StateInstance = State::StatementStartForceLstrip;
              MustLstrip = true;
            }else{
              StateInstance = State::StatementStart;
              MustLstrip = LexerConfigurationInstance.LstripBlocks;
            }
          }else if(StringView::StartsWith(OpenString, LexerConfigurationInstance.CommentOpen)){
            if(StringView::StartsWith(OpenString, LexerConfigurationInstance.CommentOpenForceLstrip)){
              StateInstance = State::CommentStartForceLstrip;
              MustLstrip = true;
            }else{
              StateInstance = State::CommentStart;
              MustLstrip = LexerConfigurationInstance.LstripBlocks;
            }
          }else if((Position == 0 || M_IN[Position - 1] == '\n') && StringView::StartsWith(OpenString, LexerConfigurationInstance.LineStatement))
            StateInstance = State::LineStart;
          else{
            // ! Wasn't actually an opening sequence
            Position += 1;
            goto Again;
          }

          std::string_view Text = StringView::Slice(M_IN, TokenStart, Position);
          if(MustLstrip)
            Text = ClearFinalLineIfWhiteSpace(Text);

          if(Text.empty()){
            // ! // don't generate empty token
            goto Again; 
          }
          return Token(Token::Kind::Text, Text);
        }
        case State::ExpressionStart: {
          StateInstance = State::ExpressionBody;
          Position += LexerConfigurationInstance.ExpressionOpen.size();
          return MakeToken(Token::Kind::ExpressionOpen);
        }
        case State::ExpressionStartForceLstrip: {
          StateInstance = State::ExpressionBody;
          Position += LexerConfigurationInstance.ExpressionOpenForceLstrip.size();
          return MakeToken(Token::Kind::ExpressionOpen);
        }
        case State::LineStart: {
          StateInstance = State::LineBody;
          Position += LexerConfigurationInstance.LineStatement.size();
          return MakeToken(Token::Kind::LineStatementOpen);
        }
        case State::StatementStart: {
          StateInstance = State::StatementBody;
          Position += LexerConfigurationInstance.StatementOpen.size();
          return MakeToken(Token::Kind::StatementOpen);
        }
        case State::StatementStartNoLstrip: {
          StateInstance = State::StatementBody;
          Position += LexerConfigurationInstance.StatementOpenNoLstrip.size();
          return MakeToken(Token::Kind::StatementOpen);
        }
        case State::StatementStartForceLstrip: {
          StateInstance = State::StatementBody;
          Position += LexerConfigurationInstance.StatementOpenForceLstrip.size();
          return MakeToken(Token::Kind::StatementOpen);
        }
        case State::CommentStart: {
          StateInstance = State::CommentBody;
          Position += LexerConfigurationInstance.CommentOpen.size();
          return MakeToken(Token::Kind::CommentOpen);
        }
        case State::CommentStartForceLstrip: {
          StateInstance = State::CommentBody;
          Position += LexerConfigurationInstance.CommentOpenForceLstrip.size();
          return MakeToken(Token::Kind::CommentOpen);
        }
        case State::ExpressionBody:
          return ScanBody(LexerConfigurationInstance.ExpressionClose, 
                    Token::Kind::ExpressionClose, LexerConfigurationInstance.ExpressionCloseForceRstrip);
        case State::LineBody:
          return ScanBody("\n", Token::Kind::LineStatementClose);
        case State::StatementBody:
          return ScanBody(LexerConfigurationInstance.StatementClose, 
                  Token::Kind::StatementClose, LexerConfigurationInstance.StatementCloseForceRstrip, LexerConfigurationInstance.TrimBlocks);
        case State::CommentBody: {
          // ! Fast-scan to comment close
          const size_t End = M_IN.substr(Position).find(LexerConfigurationInstance.CommentClose);
          if(End == std::string_view::npos){
            Position = M_IN.size();
            return MakeToken(Token::Kind::Eof);
          }

          // ! Check for trim pattern
          const bool MustRstrip = StringView::StartsWith(M_IN.substr(Position + End - 1), LexerConfigurationInstance.CommentCloseForceRstrip);

          // ! Return the entire comment in the close token
          StateInstance = State::Text;
          Position += End + LexerConfigurationInstance.CommentClose.size();
          Token TokenInstance = MakeToken(Token::Kind::CommentClose);
          if(MustRstrip || LexerConfigurationInstance.TrimBlocks) 
            SkipWhiteSpacesAndFirstNewLine();
          return TokenInstance;
        }
      }
    }

    const LexerConfiguration& GetConfiguration() const{
      return LexerConfigurationInstance;
    }
  };
}; // ! Namespace Sydonia

#endif // ! SYDONIA_LEXER_HXX

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

#ifndef SYDONIA_RENDERER_HXX
#define SYDONIA_RENDERER_HXX

#include <algorithm>
#include <numeric>
#include <string>
#include <utility>
#include <vector>

namespace Sydonia{
    // ! Class for rendering a template with data
    class Renderer : public NodeVisitor{
        using Operation = FunctionStorage::Operation;

        const RenderConfiguration RenderConfigurationInstance;
        const TemplateStorage &TemplateStorageInstance;
        const FunctionStorage &FunctionStorageInstance;
        
        const Template* CurrentTemplate;
        size_t CurrentLevel {0};
        std::vector<const Template*> TemplateStack;
        std::vector<const BlockStatementNode*> BlockStatementStack;
        
        const JSON* DataInput;
        std::ostream* OutputStream;

        JSON AdditionalData;
        JSON* CurrentLoopData = &AdditionalData["Loop"];

        std::vector<std::shared_ptr<JSON>> DataTempStack;
        std::stack<const JSON*> DataEvalStack;
        std::stack<const DataNode*> NotFoundStack;

        bool BreakRendering {false};

        static bool Truthy(const JSON* Data){
            if(Data->is_boolean())
                return Data->get<bool>();
            else if(Data->is_number())
                return (*Data != 0);
            else if(Data->is_null())
                return false;
            return !Data->empty();
        }

        void PrintData(const std::shared_ptr<JSON> Value){
            if(Value->is_string())
                *OutputStream << Value->get_ref<const JSON::string_t&>();
            else if(Value->is_number_integer())
                *OutputStream << Value->get<const JSON::number_integer_t>();
            else if(Value->is_null()){}
            else
                *OutputStream << Value->dump();
        }

        const std::shared_ptr<JSON> EvalExpressionList(const ExpressionListNode &ExpressionList){
            if(!ExpressionList.Root)
                ThrowRendererError("Empty expression", ExpressionList);
            ExpressionList.Root->Accept(*this);
            if(DataEvalStack.empty())
                ThrowRendererError("Empty expression", ExpressionList);
            else if(DataEvalStack.size() != 1)
                ThrowRendererError("Malformed expression", ExpressionList);
            const auto Result = DataEvalStack.top();
            DataEvalStack.pop();
            if(!Result){
                if(NotFoundStack.empty())
                    ThrowRendererError("Expression could not be evaluated", ExpressionList);
                auto Node = NotFoundStack.top();
                NotFoundStack.pop();
                ThrowRendererError("Variable '" + static_cast<std::string>(Node->Name) + "' not found", *Node);
            }
            return std::make_shared<JSON>(*Result);
        }

        void ThrowRendererError(const std::string &Message, const AstNode &Node){
            SourceLocation Location = GetSourceLocation(CurrentTemplate->Content, Node.Position);
            SYDONIA_THROW(RenderError(Message, Location));
        }

        void MakeResult(const JSON &&Result){
            auto ResultPointer = std::make_shared<JSON>(Result);
            DataTempStack.push_back(ResultPointer);
            DataEvalStack.push(ResultPointer.get());
        }

        template <size_t N, size_t NStart = 0, bool ThrowNotFound = true> std::array<const JSON*, N> GetArguments(const FunctionNode &Node){
            if(Node.Arguments.size() < NStart + N)
                ThrowRendererError("Function needs " + std::to_string(NStart + N) + " variables, but has only found " + std::to_string(Node.Arguments.size()), Node);
            for(size_t Iterator = NStart; Iterator < NStart + N; Iterator += 1)
                Node.Arguments[Iterator]->Accept(*this);
            if(DataEvalStack.size() < N)
                ThrowRendererError("Function needs " + std::to_string(N) + " variables, but has only found " + std::to_string(DataEvalStack.size()), Node);
            std::array<const JSON*, N> Result;
            for(size_t Iterator = 0; Iterator < N; Iterator += 1){
                Result[N - Iterator - 1] = DataEvalStack.top();
                DataEvalStack.pop();
                if(!Result[N - Iterator - 1]){
                    const auto DataNode = NotFoundStack.top();
                    NotFoundStack.pop();
                    if(ThrowNotFound)
                        ThrowRendererError("Variable '" + static_cast<std::string>(DataNode->Name) + "' not found", *DataNode);
                }
            }
            return Result;
        }

        template <bool ThrowNotFound = true> Arguments GetArgumentVector(const FunctionNode &Node){
            const size_t N = Node.Arguments.size();
            for(auto Arg : Node.Arguments)
                Arg->Accept(*this);
            if(DataEvalStack.size() < N)
                ThrowRendererError("Function needs " + std::to_string(N) + " variables, but has only found " + std::to_string(DataEvalStack.size()), Node);
            Arguments Result {N};
            for(size_t Iterator = 0; Iterator < N; Iterator += 1){
                Result[N - Iterator - 1] = DataEvalStack.top();
                DataEvalStack.pop();
                if(!Result[N - Iterator - 1]){
                    const auto DataNode = NotFoundStack.top();
                    NotFoundStack.pop();
                    if(ThrowNotFound)
                        ThrowRendererError("Variable '" + static_cast<std::string>(DataNode->Name) + "' not found", *DataNode);
                }
            }
            return Result;
        }

        void Visit(const BlockNode &Node){
            for(auto &SubNode : Node.Nodes){
                SubNode->Accept(*this);
                if(BreakRendering)
                    break;
            }
        }

        void Visit(const TextNode &Node){
            OutputStream->write(CurrentTemplate->Content.c_str() + Node.Position, Node.Length);
        }

        void Visit(const ExpressionNode &){}

        void Visit(const LiteralNode &Node){
            DataEvalStack.push(&Node.Value);
        }

        void Visit(const DataNode &Node){
            if(AdditionalData.contains(Node.Pointer))
                DataEvalStack.push(&(AdditionalData[Node.Pointer]));
            else if(DataInput->contains(Node.Pointer))
                DataEvalStack.push(&(*DataInput)[Node.Pointer]);
            else{
                // ! Try to evaluate as a no argument callback
                const auto FunctionData = FunctionStorageInstance.FindFunction(Node.Name, 0);
                if(FunctionData.OperationInstance == FunctionStorage::Operation::Callback){
                    Arguments EmptyArgs {};
                    const auto Value = std::make_shared<JSON>(FunctionData.Callback(EmptyArgs));
                    DataTempStack.push_back(Value);
                    DataEvalStack.push(Value.get());
                }else{
                    DataEvalStack.push(nullptr);
                    NotFoundStack.emplace(&Node);
                }
            }
        }

        void Visit(const FunctionNode &Node){
            switch(Node.OperationInstance){
                case Operation::Not: {
                    const auto Arguments = GetArguments<1>(Node);
                    MakeResult(!Truthy(Arguments[0]));
                } break;
                case Operation::And: {
                    MakeResult(Truthy(GetArguments<1, 0>(Node)[0]) && Truthy(GetArguments<1, 1>(Node)[0]));
                } break;
                case Operation::Or: {
                    MakeResult(Truthy(GetArguments<1, 0>(Node)[0]) || Truthy(GetArguments<1, 1>(Node)[0]));
                } break;
                case Operation::In: {
                    const auto Arguments = GetArguments<2>(Node);
                    MakeResult(std::find(Arguments[1]->begin(), Arguments[1]->end(), *Arguments[0]) != Arguments[1]->end());
                } break;
                case Operation::Equal: {
                    const auto Arguments = GetArguments<2>(Node);
                    MakeResult(*Arguments[0] == *Arguments[1]);
                } break;
                case Operation::NotEqual: {
                    const auto Arguments = GetArguments<2>(Node);
                    MakeResult(*Arguments[0] != *Arguments[1]);
                } break;
                case Operation::Greater: {
                    const auto Arguments = GetArguments<2>(Node);
                    MakeResult(*Arguments[0] > *Arguments[1]);
                } break;
                case Operation::GreaterEqual: {
                    const auto Arguments = GetArguments<2>(Node);
                    MakeResult(*Arguments[0] >= *Arguments[1]);
                } break;
                case Operation::Less: {
                    const auto Arguments = GetArguments<2>(Node);
                    MakeResult(*Arguments[0] < *Arguments[1]);
                } break;
                case Operation::LessEqual: {
                    const auto Arguments = GetArguments<2>(Node);
                    MakeResult(*Arguments[0] <= *Arguments[1]);
                } break;
                case Operation::Add: {
                    const auto Arguments = GetArguments<2>(Node);
                    if(Arguments[0]->is_string() && Arguments[1]->is_string())
                        MakeResult(Arguments[0]->get_ref<const std::string&>() + Arguments[1]->get_ref<const std::string&>());
                    else if(Arguments[0]->is_number_integer() && Arguments[1]->is_number_integer())
                        MakeResult(Arguments[0]->get<int>() + Arguments[1]->get<int>());
                    else
                        MakeResult(Arguments[0]->get<double>() + Arguments[1]->get<double>());
                } break;
                case Operation::Subtract: {
                    const auto Arguments = GetArguments<2>(Node);
                    if(Arguments[0]->is_number_integer() && Arguments[1]->is_number_integer())
                        MakeResult(Arguments[0]->get<int>() - Arguments[1]->get<int>());
                    else
                        MakeResult(Arguments[0]->get<double>() - Arguments[1]->get<double>());
                } break;
                case Operation::Multiplication: {
                    const auto Arguments = GetArguments<2>(Node);
                    if(Arguments[0]->is_number_integer() && Arguments[1]->is_number_integer())
                        MakeResult(Arguments[0]->get<int>() * Arguments[1]->get<int>());
                    else
                        MakeResult(Arguments[0]->get<double>() * Arguments[1]->get<double>());
                } break;
                case Operation::Division: {
                    const auto Arguments = GetArguments<2>(Node);
                    if(Arguments[1]->get<double>() == 0)
                        ThrowRendererError("Division by zero", Node);
                    MakeResult(Arguments[0]->get<double>() / Arguments[1]->get<double>());
                } break;
                case Operation::Power: {
                    const auto Arguments = GetArguments<2>(Node);
                    if(Arguments[0]->is_number_integer() && Arguments[1]->get<int>() >= 0){
                        int Result = static_cast<int>(std::pow(Arguments[0]->get<int>(), Arguments[1]->get<int>()));
                        MakeResult(Result);
                    }else{
                        double Result = std::pow(Arguments[0]->get<double>(), Arguments[1]->get<int>());
                        MakeResult(Result);
                    }
                } break;
                case Operation::Modulo: {
                    const auto Arguments = GetArguments<2>(Node);
                    MakeResult(Arguments[0]->get<int>() % Arguments[1]->get<int>());
                } break;
                case Operation::AtId: {
                    const auto Container = GetArguments<1, 0, false>(Node)[0];
                    Node.Arguments[1]->Accept(*this);
                    if(NotFoundStack.empty())
                        ThrowRendererError("COuld not find element with given name", Node);
                    const auto NodeID = NotFoundStack.top();
                    NotFoundStack.pop();
                    DataEvalStack.pop();
                    DataEvalStack.push(&Container->at(NodeID->Name));
                } break;
                case Operation::At: {
                    const auto Arguments = GetArguments<2>(Node);
                    if(Arguments[0]->is_object())
                        DataEvalStack.push(&Arguments[0]->at(Arguments[1]->get<std::string>()));
                    else
                        DataEvalStack.push(&Arguments[0]->at(Arguments[1]->get<int>()));
                } break;
                case Operation::Default: {
                    const auto TestArguments = GetArguments<1, 0, false>(Node)[0];
                    DataEvalStack.push(TestArguments ? TestArguments : GetArguments<1, 1>(Node)[0]);
                } break;
                case Operation::DivisibleBy: {
                    const auto Arguments = GetArguments<2>(Node);
                    const int Divisor = Arguments[1]->get<int>();
                    MakeResult((Divisor != 0) && (Arguments[0]->get<int>() % Divisor == 0));
                } break;
                case Operation::Even: {
                    MakeResult(GetArguments<1>(Node)[0]->get<int>() % 2 == 0);
                } break;
                case Operation::Exists: {
                    auto &&Name = GetArguments<1>(Node)[0]->get_ref<const std::string&>();
                    MakeResult(DataInput->contains(JSON::json_pointer(DataNode::ConvertDotToPointer(Name))));
                } break;
                case Operation::ExistsInObject: {
                    const auto Arguments = GetArguments<2>(Node);
                    auto &&Name = Arguments[1]->get_ref<const std::string&>();
                    MakeResult(Arguments[0]->find(Name) != Arguments[0]->end());
                } break;
                case Operation::First: {
                    const auto Result = &GetArguments<1>(Node)[0]->front();
                    DataEvalStack.push(Result);
                } break;
                case Operation::Float: {
                    MakeResult(std::stod(GetArguments<1>(Node)[0]->get_ref<const std::string&>()));
                } break;
                case Operation::Int: {
                    MakeResult(std::stoi(GetArguments<1>(Node)[0]->get_ref<const std::string&>()));
                } break;
                case Operation::Last: {
                    const auto Result = &GetArguments<1>(Node)[0]->back();
                    DataEvalStack.push(Result);
                } break;
                case Operation::Length: {
                    const auto Value = GetArguments<1>(Node)[0];
                    if(Value->is_string())
                        MakeResult(Value->get_ref<const std::string&>().length());
                    else
                        MakeResult(Value->size());
                } break;
                case Operation::Lower: {
                    std::string Result = GetArguments<1>(Node)[0]->get<std::string>();
                    std::transform(Result.begin(), Result.end(), Result.begin(), ::tolower);
                    MakeResult(std::move(Result));
                } break;
                case Operation::Max: {
                    const auto Arguments = GetArguments<1>(Node);
                    const auto Result = std::max_element(Arguments[0]->begin(), Arguments[0]->end());
                    DataEvalStack.push(&(*Result));
                } break;
                case Operation::Min: {
                    const auto Arguments = GetArguments<1>(Node);
                    const auto Result = std::min_element(Arguments[0]->begin(), Arguments[0]->end());
                    DataEvalStack.push(&(*Result));
                } break;
                case Operation::Odd: {
                    MakeResult(GetArguments<1>(Node)[0]->get<int>() % 2 != 0);
                } break;
                case Operation::Range: {
                    std::vector<int> Result(GetArguments<1>(Node)[0]->get<int>());
                    std::iota(Result.begin(), Result.end(), 0);
                    MakeResult(std::move(Result));
                } break;
                case Operation::Round: {
                    const auto Arguments = GetArguments<2>(Node);
                    const int Precision = Arguments[1]->get<int>();
                    const double Result = std::round(Arguments[0]->get<double>() * std::pow(10.0, Precision)) / std::pow(10.0, Precision);
                    if(Precision == 0)
                        MakeResult(int(Result));
                    else
                        MakeResult(Result);
                } break;
                case Operation::Sort: {
                    auto ResultPointer = std::make_shared<JSON>(GetArguments<1>(Node)[0]->get<std::vector<JSON>>());
                    std::sort(ResultPointer->begin(), ResultPointer->end());
                    DataTempStack.push_back(ResultPointer);
                    DataEvalStack.push(ResultPointer.get());
                } break;
                case Operation::Upper: {
                    std::string Result = GetArguments<1>(Node)[0]->get<std::string>();
                    std::transform(Result.begin(), Result.end(), Result.begin(), ::toupper);
                    MakeResult(std::move(Result));
                } break;
                case Operation::IsBoolean: {
                    MakeResult(GetArguments<1>(Node)[0]->is_boolean());
                } break;
                case Operation::IsNumber: {
                    MakeResult(GetArguments<1>(Node)[0]->is_number());
                } break;
                case Operation::IsInteger: {
                    MakeResult(GetArguments<1>(Node)[0]->is_number_integer());
                } break;
                case Operation::IsFloat: {
                    MakeResult(GetArguments<1>(Node)[0]->is_number_float());
                } break;
                case Operation::IsObject: {
                    MakeResult(GetArguments<1>(Node)[0]->is_object());
                } break;
                case Operation::IsArray: {
                    MakeResult(GetArguments<1>(Node)[0]->is_array());
                } break;
                case Operation::IsString: {
                    MakeResult(GetArguments<1>(Node)[0]->is_string());
                } break;
                case Operation::Callback: {
                    auto Arguments = GetArgumentVector(Node);
                    MakeResult(Node.Callback(Arguments));
                } break;
                case Operation::Super: {
                    const auto Arguments = GetArgumentVector(Node);
                    const size_t OldLevel = CurrentLevel;
                    const size_t LevelDiff = (Arguments.size() != 1) ? Arguments[0]->get<int>() : 1;
                    const size_t Level = CurrentLevel + LevelDiff;

                    if(BlockStatementStack.empty())
                        ThrowRendererError("Super() call is not withing a block", Node);
                    
                    if(Level < 1 || Level > TemplateStack.size() - 1)
                        ThrowRendererError("Level of Super() call does not match parent templates (between 1 and " + std::to_string(TemplateStack.size() - 1) + ")", Node);

                    const auto CurrentBlockStatement = BlockStatementStack.back();
                    const Template* NewTemplate = TemplateStack.at(Level);
                    const Template* OldTemplate = CurrentTemplate;
                    const auto BlockIterator = NewTemplate->BlockStorage.find(CurrentBlockStatement->Name);
                    if(BlockIterator != NewTemplate->BlockStorage.end()){
                        CurrentTemplate = NewTemplate;
                        CurrentLevel = Level;
                        BlockIterator->second->Block.Accept(*this);
                        CurrentLevel = OldLevel;
                        CurrentTemplate = OldTemplate;
                    }else
                        ThrowRendererError("Could not find block with name '" + CurrentBlockStatement->Name + "'", Node);
                    MakeResult(nullptr);
                } break;
                case Operation::Join: {
                    const auto Arguments = GetArguments<2>(Node);
                    const auto Separator = Arguments[1]->get<std::string>();
                    std::ostringstream Stream;
                    std::string Sep;
                    for(const auto &Value : *Arguments[0]){
                        Stream << Sep;
                        if(Value.is_string())
                            Stream << Value.get<std::string>();
                        else
                            Stream << Value.dump();
                        Sep = Separator;
                    }
                    MakeResult(Stream.str());
                } break;
                case Operation::ParenLeft:
                case Operation::ParenRight:
                case Operation::None:
                    break;
            }
        }

        void Visit(const ExpressionListNode &Node){
            PrintData(EvalExpressionList(Node));
        }

        void Visit(const StatementNode &){}

        void Visit(const ForStatementNode &){}

        void Visit(const ForArrayStatementNode &Node){
            const auto Result = EvalExpressionList(Node.Condition);
            if(!Result->is_array())
                ThrowRendererError("Object must be an array", Node);
            if(!CurrentLoopData->empty()){
                auto Temp = *CurrentLoopData;
                (*CurrentLoopData)["Parent"] = std::move(Temp);
            }
            size_t Index = 0;
            (*CurrentLoopData)["IsFirst"] = true;
            (*CurrentLoopData)["IsLast"] = (Result->size() <= 1);
            for(auto Iterator = Result->begin(); Iterator != Result->end(); ++Iterator){
                AdditionalData[static_cast<std::string>(Node.Value)] = *Iterator;
                (*CurrentLoopData)["Index"] = Index;
                (*CurrentLoopData)["Index1"] = Index + 1;
                if(Index == 1)
                    (*CurrentLoopData)["IsFirst"] = false;
                if(Index == Result->size() - 1)
                    (*CurrentLoopData)["IsLast"] = true;
                Node.Body.Accept(*this);
                ++Index;
            }

            AdditionalData[static_cast<std::string>(Node.Value)].clear();
            if(!(*CurrentLoopData)["Parent"].empty()){
                const auto Temp = (*CurrentLoopData)["Parent"];
                *CurrentLoopData = std::move(Temp);
            }else
                CurrentLoopData = &AdditionalData["Loop"];
        }

        void Visit(const ForObjectStatementNode &Node){
            const auto Result = EvalExpressionList(Node.Condition);
            if(!Result->is_object())
                ThrowRendererError("Object must be an object", Node);
            if(!CurrentLoopData->empty())
                (*CurrentLoopData)["Parent"] = std::move(*CurrentLoopData);
            size_t Index = 0;
            (*CurrentLoopData)["IsFirst"] = true;
            (*CurrentLoopData)["IsLast"] = (Result->size() <= 1);
            for(auto Iterator = Result->begin(); Iterator != Result->end(); ++Iterator){
                AdditionalData[static_cast<std::string>(Node.Key)] = Iterator.key();
                AdditionalData[static_cast<std::string>(Node.Value)] = Iterator.value();
                (*CurrentLoopData)["Index"] = Index;
                (*CurrentLoopData)["Index1"] = Index + 1;
                if(Index == 1)
                    (*CurrentLoopData)["IsFirst"] = false;
                if(Index == Result->size() - 1)
                    (*CurrentLoopData)["IsLast"] = true;
                Node.Body.Accept(*this);
                ++Index;
            }
            AdditionalData[static_cast<std::string>(Node.Key)].clear();
            AdditionalData[static_cast<std::string>(Node.Value)].clear();
            if(!(*CurrentLoopData)["Parent"].empty())
                *CurrentLoopData = std::move((*CurrentLoopData)["Parent"]);
            else
                CurrentLoopData = &AdditionalData["Loop"];
        }

        void Visit(const IfStatementNode &Node){
            const auto Result = EvalExpressionList(Node.Condition);
            if(Truthy(Result.get()))
                Node.TrueStatement.Accept(*this);
            else if(Node.HasFalseStatement)
                Node.FalseStatement.Accept(*this);
        }

        void Visit(const IncludeStatementNode &Node){
            auto SubRenderer = Renderer(RenderConfigurationInstance, TemplateStorageInstance, FunctionStorageInstance);
            const auto IncludedTemplateIterator = TemplateStorageInstance.find(Node.File);
            if(IncludedTemplateIterator != TemplateStorageInstance.end())
                SubRenderer.RenderTo(*OutputStream, IncludedTemplateIterator->second, *DataInput, &AdditionalData);
            else if(RenderConfigurationInstance.ThrowAtMissingIncludes)
                ThrowRendererError("Include '" + Node.File + "' not found", Node);
        }
        
        void Visit(const ExtendsStatementNode &Node){
            const auto IncludedTemplateIterator = TemplateStorageInstance.find(Node.File);
            if(IncludedTemplateIterator != TemplateStorageInstance.end()){
                const Template* ParentTemplate = &IncludedTemplateIterator->second;
                RenderTo(*OutputStream, *ParentTemplate, *DataInput, &AdditionalData);
                BreakRendering = true;
            }else if(RenderConfigurationInstance.ThrowAtMissingIncludes)
                ThrowRendererError("Extends '" + Node.File + "' not found", Node);
        }

        void Visit(const BlockStatementNode &Node){
            const size_t OldLevel = CurrentLevel;
            CurrentLevel = 0;
            CurrentTemplate = TemplateStack.front();
            const auto BlockIterator = CurrentTemplate->BlockStorage.find(Node.Name);
            if(BlockIterator != CurrentTemplate->BlockStorage.end()){
                BlockStatementStack.emplace_back(&Node);
                BlockIterator->second->Block.Accept(*this);
                BlockStatementStack.pop_back();
            }
            CurrentLevel = OldLevel;
            CurrentTemplate = TemplateStack.back();
        }

        void Visit(const SetStatementNode &Node){
            std::string Pointer = Node.Key;
            ReplaceSubString(Pointer, ".", "/");
            Pointer = "/" + Pointer;
            AdditionalData[JSON::json_pointer(Pointer)] = *EvalExpressionList(Node.Expression);
        }

        public:
            Renderer(const RenderConfiguration &RenderConfigurationLocal, const TemplateStorage &TemplateStorageLocal, const FunctionStorage &FunctionStorageLocal)
                : RenderConfigurationInstance(RenderConfigurationLocal), TemplateStorageInstance(TemplateStorageLocal), FunctionStorageInstance(FunctionStorageLocal){}
            
            void RenderTo(std::ostream &Stream, const Template &TemplateLocal, const JSON &Data, JSON* LoopData = nullptr){
                OutputStream = &Stream;
                CurrentTemplate = &TemplateLocal;
                DataInput = &Data;
                if(LoopData){
                    AdditionalData = *LoopData;
                    CurrentLoopData = &AdditionalData["Loop"];
                }
                TemplateStack.emplace_back(CurrentTemplate);
                CurrentTemplate->Root.Accept(*this);
                DataTempStack.clear();
            }
    };
}; // ! Sydonia namespace

#endif // ! SYDONIA_RENDERER_HXX

namespace Sydonia{
    // ! Class for changing the configuration
    class Environment{
        std::string OutputPath;
        std::string InputPath;

        LexerConfiguration LexerConfigurationInstance;
        ParserConfiguration ParserConfigurationInstance;
        RenderConfiguration RenderConfigurationInstance;

        FunctionStorage FunctionStorageInstance;
        TemplateStorage TemplateStorageInstance;

        public:
            Environment(): Environment(""){}
            
            explicit Environment(const std::string &GlobalPath): InputPath(GlobalPath), OutputPath(GlobalPath){}

            Environment(const std::string &InputPathLocal, const std::string &OutputPathLocal): InputPath(InputPathLocal), OutputPath(OutputPathLocal){}

            // ! Sets the opener and closer for template statements
            void SetStatement(const std::string &Open, const std::string &Close){
                LexerConfigurationInstance.StatementOpen = Open;
                LexerConfigurationInstance.StatementOpenNoLstrip = Open + "+";
                LexerConfigurationInstance.StatementOpenForceLstrip = Open + "-";
                LexerConfigurationInstance.StatementClose = Close;
                LexerConfigurationInstance.StatementCloseForceRstrip = "-" + Close;
                LexerConfigurationInstance.UpdateOpenCharacters();
            }

            // ! Sets the opener for template line statement
            void SetLineStatement(const std::string &Open){
                LexerConfigurationInstance.LineStatement = Open;
                LexerConfigurationInstance.UpdateOpenCharacters();
            }

            // ! Sets the opener and closer for template expressions
            void SetExpression(const std::string &Open, const std::string &Close){
                LexerConfigurationInstance.ExpressionOpen = Open;
                LexerConfigurationInstance.ExpressionOpenForceLstrip = Open + "-";
                LexerConfigurationInstance.ExpressionClose = Close;
                LexerConfigurationInstance.ExpressionCloseForceRstrip = "-" + Close;
                LexerConfigurationInstance.UpdateOpenCharacters();
            }

            // ! Sets the opener and closer for template comments
            void SetComment(const std::string &Open, const std::string &Close){
                LexerConfigurationInstance.CommentOpen = Open;
                LexerConfigurationInstance.CommentOpenForceLstrip = Open + "-";
                LexerConfigurationInstance.CommentClose = Close;
                LexerConfigurationInstance.CommentCloseForceRstrip = "-" + Close;
                LexerConfigurationInstance.UpdateOpenCharacters();
            }

            // ! Sets whether to remove the first newline after a block
            void SetTrimBlocks(bool TrimBlocks){
                LexerConfigurationInstance.TrimBlocks = TrimBlocks;
            }

            // ! Sets whether to strip the spaces and tabs from the start of a line to a block
            void SetLstripBlocks(bool LstripBlocks){
                LexerConfigurationInstance.LstripBlocks = LstripBlocks;
            }

            // ! Sets the element notation syntax
            void SetSearchIncludedTemplatesInFiles(bool SearchInFiles){
                ParserConfigurationInstance.SearchIncludedTemplatesInFiles = SearchInFiles;
            }

            // ! Sets whether a missing include will throw an error
            void SetThrowAtMissingIncludes(bool WillThrow){
                RenderConfigurationInstance.ThrowAtMissingIncludes = WillThrow;
            }

            Template Parse(std::string_view Input){
                Parser ParserLocal(ParserConfigurationInstance, LexerConfigurationInstance, TemplateStorageInstance, FunctionStorageInstance);
                return ParserLocal.Parse(Input);
            }

            Template ParseTemplate(const std::string &Filename){
                Parser ParserLocal(ParserConfigurationInstance, LexerConfigurationInstance, TemplateStorageInstance, FunctionStorageInstance);
                auto Result = Template(ParserLocal.LoadFile(InputPath + static_cast<std::string>(Filename)));
                ParserLocal.ParseIntoTemplate(Result, InputPath + static_cast<std::string>(Filename));
                return Result;
            }

            Template ParseFile(const std::string &Filename){
                return ParseTemplate(Filename);
            }

            std::string Render(std::string_view Input, const JSON &Data){
                return Render(Parse(Input), Data);
            }

            std::string Render(const Template &TemplateLocal, const JSON &Data){
                std::stringstream Stream;
                RenderTo(Stream, TemplateLocal, Data);
                return Stream.str();
            }

            std::string RenderFile(const std::string &Filename, const JSON &Data){
                return Render(ParseTemplate(Filename), Data);
            }

            std::string RenderFileWithJsonFile(const std::string &Filename, const std::string &FilenameData){
                const JSON Data = LoadJSON(FilenameData);
                return RenderFile(Filename, Data);
            }

            void Write(const std::string &Filename, const JSON &Data, const std::string &FilenameOut){
                std::ofstream File(OutputPath + FilenameOut);
                File << RenderFile(Filename, Data);
                File.close();
            }

            void Write(const Template &TemplateLocal, const JSON &Data, const std::string &FilenameOut){
                std::ofstream File(OutputPath + FilenameOut);
                File << Render(TemplateLocal, Data);
                File.close();
            }

            void WriteWithJsonFile(const std::string &Filename, const std::string &FilenameData, const std::string &FilenameOut){
                const JSON Data = LoadJSON(FilenameData);
                Write(Filename, Data, FilenameOut);
            }

            void WriteWithJsonFile(const Template &TemplateLocal, const std::string &FilenameData, const std::string &FilenameOut){
                const JSON Data = LoadJSON(FilenameData);
                Write(TemplateLocal, Data, FilenameOut);
            }

            std::ostream &RenderTo(std::ostream &Stream, const Template &TemplateLocal, const JSON &Data){
                Renderer(RenderConfigurationInstance, TemplateStorageInstance, FunctionStorageInstance).RenderTo(Stream, TemplateLocal, Data);
                return Stream;
            }

            std::string LoadFile(const std::string &Filename){
                Parser ParserLocal(ParserConfigurationInstance, LexerConfigurationInstance, TemplateStorageInstance, FunctionStorageInstance);
                return ParserLocal.LoadFile(InputPath + Filename);
            }

            JSON LoadJSON(const std::string &Filename){
                std::ifstream File;
                File.open(InputPath + Filename);
                if(File.fail())
                    SYDONIA_THROW(FileError("Failed accessing file at '" + InputPath + Filename + "'"));
                return JSON::parse(std::istreambuf_iterator<char>(File), std::istreambuf_iterator<char>());
            }

            // ! Adds a variadic callback
            void AddCallback(const std::string &Name, const CallbackFunction &Callback){
                AddCallback(Name, -1, Callback);
            }

            // ! Adds a variadic void callback
            void AddVoidCallback(const std::string &Name, const VoidCallbackFunction &Callback){
                AddVoidCallback(Name, -1, Callback);
            }

            // ! Adds a callback with given number of arguments
            void AddCallback(const std::string &Name, int NumberArgs, const CallbackFunction &Callback){
                FunctionStorageInstance.AddCallback(Name, NumberArgs, Callback);
            }

            // ! Adds a void callback with given number of arguments
            void AddVoidCallback(const std::string &Name, int NumberArgs, const VoidCallbackFunction &Callback){
                FunctionStorageInstance.AddCallback(Name, NumberArgs, [Callback](Arguments &Args){
                    Callback(Args);
                    return JSON();
                });
            }

            // ! Includes a template with a given name into the environment
            // ! then a template can be rendered in another template using
            // ! the include "<Name>" syntax
            void IncludeTemplate(const std::string &Name, const Template &TemplateLocal){
                TemplateStorageInstance[Name] = TemplateLocal;
            }

            // ! Sets a function that is called when an included file is not found
            void SetIncludeCallback(const std::function<Template(const std::string &, const std::string &)>& Callback){
                ParserConfigurationInstance.IncludeCallback = Callback;
            }
    };

    // ! Render with default settings to a string
    inline std::string Render(std::string_view Input, const JSON &Data){
        return Environment().Render(Input, Data);
    }

    // ! Render with default settings to the given output stream
    inline void RenderTo(std::ostream &Stream, std::string_view Input, const JSON &Data){
        Environment EnvironmentLocal;
        EnvironmentLocal.RenderTo(Stream, EnvironmentLocal.Parse(Input), Data);
    }
}; // ! Sydonia namespace

#endif // ! SYDONIA_ENVIRONMENT_HXX

#endif // ! SYDONIA_HXX
