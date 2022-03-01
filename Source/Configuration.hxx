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

#include "Template.hxx"

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