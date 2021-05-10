
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
        bool _object_owned;  // This may not be needed anymore, testing
        ValuePtr(Value& v) : _ptr(v.clone()),_object_owned (false)
        {
        }
        ValuePtr(Value *ptr, bool owned=false) : _ptr(ptr), _object_owned(owned)
        {
        }
        ValuePtr(const ValuePtr &rhs) : _ptr(rhs._ptr), _object_owned(rhs._object_owned)
        {
        }
        ~ValuePtr(){
         //   if (!_object_owned) delete _ptr;
        }
        // The ownership is handled in Object
        // it deletes unowned value there
//        inline Value *operator*() { return get(); }
//      inline Value *operator->() { return get(); }
  //      template <typename TValue = Value>
  //      inline TValue *get() { return (TValue *)_ptr; };
        template <typename TValue = Value>
        inline TValue &ref() { return (TValue &)*_ptr; }
    };



    
    

}