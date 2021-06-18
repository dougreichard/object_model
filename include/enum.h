#pragma once

#include <unordered_map>
#include "symbol.h"
#include "value.h"

namespace obj
{
    template <typename Host>
    struct Enum
    {
        struct VP { 
            ValuePtr value; 
            VP(const char* name, Value& v ) : value(v.clone()) {
                UserSymbol s(name);
                Host::values.insert(std::make_pair((Symbol)s,value));
            }
        };
        static inline std::unordered_map<Symbol, ValuePtr> values;
    };
}