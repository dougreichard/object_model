#pragma once
#include "visitor.h"
#include "value.h"
#include "metadata.h"
#include "base_types.h"

#include <unordered_map>
#include <unordered_set>


#define MAKE_OBJECT_POLY(T)                                                                     \
    virtual Object &poly_set(const Symbol &key, Value &value) { return this->set(key, value); } \
    virtual Value &poly_get(const Symbol &key) { return this->get(key); }                       \
    virtual Object &poly_remove(const Symbol &key) { return this->remove(key); }


namespace obj
{
    typedef std::unordered_set<Symbol> Keywords;
    typedef std::unordered_map<Symbol, ValuePtr> KeyValues;
    struct Object : Value
    {
        MAKE_VISITABLE(Object, Value)
        std::shared_ptr<KeyValues> _kv;
        std::shared_ptr<Keywords> _kw;
        Object() : Value(object_type)
        {
            init();
        }
        Object(const Symbol &type) : Value(type)
        {
            init();
        }
        Object(const Metadata &meta, const Symbol &type=object_type) : Value(type)
        {
            init();
            from_metadata(meta);
        }
        void init()
        {
            _kv = std::make_shared<KeyValues>();
            _kw = std::make_shared<Keywords>();
        }
        void from_metadata(const Metadata &meta)
        {
            for (auto p : meta.props)
            {
                if (p.owned)
                    set_owned(p.name, p.value.ref());
                else
                    set(p.name, p.value.ref());
            }
        }
        // Copy constructor all copies share the same key/values
        Object(const Object &rhs) : Value(rhs._type)
        {
            _kv = rhs._kv;
            _kw = rhs._kw;
        }

        virtual ~Object()
        {
            if (_kv.use_count() < 2)
            {
                for (auto i = _kv->begin(); i != _kv->end(); i++)
                {
                    // if (!i->second._object_owned)
                    // {
                    //     delete i->second._ptr;
                    // }
                }
            }
        }

        inline Object &set_owned(const Symbol &key, Value &value)
        {
          //  _kv->insert_or_assign(key, ValuePtr(&value, true));
            return *this;
        }

        // Map emplace is really slow
        // This make sure it only does this when appropriate
        template <typename TValue = Value>
        inline Object &set(const Symbol &key, TValue &value)
        {
            Value &v = get(key);
            if (v._type._key != value._type._key)
            {
                _kv->insert_or_assign(key, ValuePtr(value.clone()));
            }
            else if (v._type._key == INT_SYMBOL)
            {
                ((Int &)v) = (Int &)value;
            }
            else if (v._type._key == FLOAT_SYMBOL)
            {
                ((Float &)v) = (Float &)value;
            }
            else if (v._type._key == STRING_SYMBOL)
            {
                ((String &)v) = (String &)value;
            }
            else if (v._type._key == UNDEFINED_SYMBOL)
            {
                //do nothing
            }
            else
            {
                // Objects and array should clone
                _kv->insert_or_assign(key, ValuePtr(value.clone()));
            }

            return *this;
        }

        template <typename TValue = Value>
        inline TValue &get(const Symbol &key)
        {
            KeyValues::iterator v = _kv->find(key);
            if (v == _kv->end())
            {
                return (TValue&)Undefined;
            }
            return (TValue&)v->second.ref();
        }
        inline Object &remove(const Symbol &key)
        {
            auto i = _kv->find(key);
            ValuePtr v = i->second;
          //  if (!v._object_owned)
          //      delete v._ptr;
            _kv->erase(key);

            return *this;
        }
        inline bool has_value(const Symbol &key)
        {
            return _kv->find(key) != _kv->end();
        }

        inline void add_role(const Symbol &role)
        {
            _kw->insert(role);
        }
        inline void remove_role(const Symbol &role)
        {
            _kw->erase(role);
        }
        inline bool has_role(const Symbol &role)
        {
            return _kw->find(role) != _kw->end();
        }
        // virtual std::shared_ptr<Value> make_shared()
        // {
        //     return std::shared_ptr<Value>(new Object(*this));
        // }
        virtual Value* clone()
        {
            Object* clone = new Object(this->_type);
            for(auto [k,v] : *_kv) {
                clone->set(k,v.ref());
            }

            return clone;
        }

        // These are virtual/poly morphic, but slower then the inlines
        virtual Object &poly_set(const Symbol &key, Value &value)
        {
            return this->set(key, value);
        }

        virtual Value &poly_get(const Symbol &key)
        {
            return this->get(key);
        }
        virtual Object &poly_remove(const Symbol &key)
        {
            return this->remove(key);
        }
    };

}