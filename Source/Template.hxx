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

#ifndef SYDONIA_TEMPLATE_HXX
#define SYDONIA_TEMPLATE_HXX

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "Node.hxx"
#include "Statistics.hxx"

namespace Sydonia{
    // ! The main Sydonia Template
    struct Template{
        BlockNode Root;
        std::string Content;
        std::map<std::string, std::shared_ptr<BlockStatementNode>> BlockStorage;
        
        explicit Template(){}
        explicit Template(const std::string &ContentLocal): Content(ContentLocal){}

        // ! Return number of variables (total number, not distinct ones) in the template
        int CountVariables(){
            auto StatisticVisitor = StatisticsVisitor();
            Root.Accept(StatisticVisitor);
            return StatisticVisitor.VariableCounter;
        }
    };

    using TemplateStorage = std::map<std::string, Template>;
}; // ! Sydonia namespace

#endif // ! SYDONIA_TEMPLATE_HXX