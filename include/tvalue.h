#pragma once
#include "symbol.h"
#include "value.h"

namespace obj {
    template <typename T, const Symbol &name>
    struct TValue : Value
    {
        typedef T value_type;
        TValue() : Value(name), _value (T()) {}
        TValue(const T &value) : Value(name), _value(value) {}
        TValue(const TValue &rhs) : Value(rhs._type), _value(rhs._value) {}
        T _value;
//        static const inline TValue default_value = T(0);
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
        //MAKE_VISITABLE(TValue, Value)
    };

}