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

#include "Configuration.hxx"
#include "Exceptions.hxx"
#include "Node.hxx"
#include "Template.hxx"
#include "Utilities.hxx"

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