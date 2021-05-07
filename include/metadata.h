#pragma once

#include <vector>
#include "symbol.h"
#include "value.h"

namespace obj
{
    // This is for defining metadata
    // a list of properties for a class
    struct Property
    {
        const Symbol &name;
        const Symbol &type;
        ValuePtr value;
        bool owned = false;
    };

    // Metadata information
    struct Metadata
    {
        Metadata(std::initializer_list<Property> p) : props(p) {
            
        }
        std::vector<Property> props;
    };
}