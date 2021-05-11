
#pragma once
#include <string>
#include "symbol.h"
#include "scope.h"

namespace obj
{

    struct Value
#if defined(USE_VISITOR) || defined(USE_VISITOR_POLY)
        : IVisitable
    {
        MAKE_VISITABLE(Value, IVisitable)
#else
    {
#endif
        Value(const Symbol &type) : _type(type) {}
        virtual ~Value() {}
        const Symbol &_type;
        virtual Value* clone() = 0;
    };
    
    struct ValuePtr
    {
        std::shared_ptr<Value> _ptr;
        ValuePtr(Value& v) : _ptr(v.clone())
        {
        }
        ValuePtr(Value *ptr) : _ptr(ptr)
        {
        }
        ValuePtr(const ValuePtr &rhs) : _ptr(rhs._ptr)
        {
        }
        ~ValuePtr(){
         //   if (!_object_owned) delete _ptr;
        }
        template <typename TValue = Value>
        inline TValue &ref() { return (TValue &)*_ptr; }
    };



    
    

}