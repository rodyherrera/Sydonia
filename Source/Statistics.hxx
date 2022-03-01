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

#include "Node.hxx"

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
