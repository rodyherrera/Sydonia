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

#include "Exceptions.hxx"

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