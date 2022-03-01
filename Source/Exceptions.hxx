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