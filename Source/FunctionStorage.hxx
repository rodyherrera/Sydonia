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