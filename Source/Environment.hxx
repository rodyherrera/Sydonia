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

#include "Configuration.hxx"
#include "FunctionStorage.hxx"
#include "Parser.hxx"
#include "Renderer.hxx"
#include "Template.hxx"
#include "Utilities.hxx"

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