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