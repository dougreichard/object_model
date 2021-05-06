
#pragma once
#include <string>
#include "symbol.h"
#include "scope.h"

namespace obj
{

    struct Value;
    struct ValuePtr
    {
        Value *_ptr;
        bool _owned;
        ValuePtr(Value *ptr, bool owned) : _ptr(ptr), _owned(owned)
        {
        }
        ValuePtr(const ValuePtr &rhs) : _ptr(rhs._ptr), _owned(rhs._owned)
        {
        }
        // ~ValuePtr() {
        //     if (!_owned) delete _ptr;
        // }
        // The ownership is handled in Object
        // it deletes unowned value there
        inline Value *operator*() { return get(); }
        inline Value *operator->() { return get(); }
        inline Value *get() { return _ptr; };
        inline Value &ref() { return *get(); }
    };

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
        virtual std::shared_ptr<Value> make_shared() = 0;
        virtual ValuePtr clone() = 0;
    };

    

}