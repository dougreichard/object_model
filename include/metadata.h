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
        std::vector<Property> props;
    };
}