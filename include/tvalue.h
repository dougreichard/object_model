#pragma once
#include "symbol.h"
#include "value.h"

namespace obj {
    template <typename T, const Symbol &name>
    struct TValue : Value
    {
        TValue() : Value(name) {}
        TValue(const T &value) : Value(name), _value(value) {}
        T _value;
        inline operator const T &() const { return _value; }
        inline operator T &() { return _value; }
        inline TValue &operator=(const TValue &value)
        {
            _value = value;
            return *this;
        }
        inline TValue &operator=(const T &value)
        {
            _value = value;
            return *this;
        }
        virtual std::shared_ptr<Value> make_shared()
        {
            return std::shared_ptr<Value>(new TValue(*this));
        }
        virtual ValuePtr clone()
        {
            return ValuePtr(new TValue(*this), false);
        }
        MAKE_VISITABLE(TValue, Value)
    };

}