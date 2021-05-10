
#pragma once
#include "visitor.h"
#include "value.h"
#include "base_types.h"

#include <vector>

namespace obj {
    typedef std::vector<ValuePtr> ValueArray;
    struct Array : Value
    {
        MAKE_VISITABLE(Array, Value)
        std::shared_ptr<ValueArray> _vec;
        Array() : Value(array_type)
        {
            _vec = std::make_shared<ValueArray>();
        }
        // Copy constructor all copies share the same key/values
        Array(const Array &rhs) : Value(rhs._type)
        {
            _vec = rhs._vec;
        }
        ~Array()
        {
            // if (_vec.use_count() < 2)
            // {
            //     for (auto i = _vec->begin(); i != _vec->end(); i++)
            //     {
            //         if (!i->_object_owned)
            //         {
            //             delete i->_ptr;
            //         }
            //     }
            // }
        }
        
        virtual Value* clone()
        {
            return new Array(*this);
        }
        Array &push(Value &v)
        {
            _vec->push_back(ValuePtr(v.clone(), false));
            return *this;
        }
        inline Value &operator[](size_t idx)
        {
            return _vec->at(idx).ref();
        }
        inline const size_t size() const
        {
            return _vec->size();
        }
    };
}