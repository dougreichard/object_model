#pragma once

#include <vector>
#include "symbol.h"
#include "value.h"

namespace obj
{
    // This is for defining metadata
    // a list of properties for a class
    struct EnumConst
    {
        const Symbol &name;
        ValuePtr value;
    };

    // Metadata information
    struct EnumSet
    {
        EnumSet(std::initializer_list<EnumConst> p) : props(p) {
            
        }
        std::vector<EnumConst> props;
    };

    
}