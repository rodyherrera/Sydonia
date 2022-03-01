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

#include "FunctionStorage.hxx"
#include "Utilities.hxx"

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