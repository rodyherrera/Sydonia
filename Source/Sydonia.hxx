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

#include "Environment.hxx"
#include "Exceptions.hxx"
#include "Parser.hxx"
#include "Renderer.hxx"
#include "Template.hxx"

#endif // ! SYDONIA_HXX
