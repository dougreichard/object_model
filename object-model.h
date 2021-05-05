#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <memory>

namespace obj
{
    /// Base interface fo a visitor
    struct IVisitor
    {
        virtual ~IVisitor() {}
    };
    /// Base for anything that can be visited
    struct IVisitable
    {
        virtual ~IVisitable() {}
        virtual void accept(IVisitor *v) {}
    };
    template <typename T, typename B = IVisitable>
    struct BVisitable : public B
    {
        void accept(IVisitor *v)
        {
            IVisit<T> *visit_me = dynamic_cast<IVisit<T> *>(v);
            if (visit_me)
                visit_me->visit((T &)*this);
            else
                B::accept(v);
        }
    };
#define MAKE_VISITABLE(T)                                   \
    void accept(IVisitor *v)                                \
    {                                                       \
        IVisit<T> *visit_me = dynamic_cast<IVisit<T> *>(v); \
        if (visit_me)                                       \
            visit_me->visit((T &)*this);                    \
    }
#define MAKE_VISITABLE_POLY(T, B)                           \
    void accept(IVisitor *v)                                \
    {                                                       \
        IVisit<T> *visit_me = dynamic_cast<IVisit<T> *>(v); \
        if (visit_me)                                       \
            visit_me->visit((T &)*this);                    \
        else                                                \
            B::accept(v);                                   \
    }
//////////////////////////////////////////////////
// This adds virtual version of get, set, remove
// these are slower than the inline versions
//
#define MAKE_OBJECT_POLY(T)                                                                     \
    virtual Object &poly_set(const Symbol &key, Value &value) { return this->set(key, value); } \
    virtual Value &poly_get(const Symbol &key) { return this->get(key); }                       \
    virtual Object &poly_remove(const Symbol &key) { return this->remove(key); }

    // Used to build visitors
    template <typename T>
    struct IVisit
    {
        virtual void visit(T &obj) = 0;
    };
    // Human string, system int
    struct Symbol
    {
        static std::unordered_map<std::string, Symbol *> symbols;
        static Symbol *get_or_create_symbol(const char *name);

        std::string _name;
        uint16_t _key;
        Symbol(const char *name, uint16_t key) : _name(name), _key(key)
        {
            Symbol::symbols.insert({std::string(name), this});
            //printf("-%s-", name);
        }
        operator const uint16_t &() const { return _key; }
        operator const std::string &() const { return _name; }
    };

#ifndef __DEFINE_STATIC__
    extern const uint16_t UNDEFINED_SYMBOL;
    extern const uint16_t OBJECT_SYMBOL;
    extern const uint16_t INT_SYMBOL;
    extern const uint16_t FLOAT_SYMBOL;
    extern const uint16_t STRING_SYMBOL;
    extern const uint16_t ARRAY_SYMBOL;
    extern const uint16_t LAST_BASE_SYMBOL;
    extern uint16_t SYSTEM_SYMBOLS;
    extern uint16_t MAX_SYMBOLS; // MAX SYSTEM and USER SYMBOL
    extern uint16_t USER_SYMBOLS;
    extern const Symbol undefined_type;
    extern const Symbol object_type;
    extern const Symbol array_type;
#else
    std::unordered_map<std::string, obj::Symbol *> obj::Symbol::symbols;
    extern const uint16_t UNDEFINED_SYMBOL = 0;
    extern const uint16_t OBJECT_SYMBOL = 1;
    extern const uint16_t INT_SYMBOL = 2;
    extern const uint16_t FLOAT_SYMBOL = 3;
    extern const uint16_t STRING_SYMBOL = 4;
    extern const uint16_t ARRAY_SYMBOL = 5;
    extern const uint16_t LAST_BASE_SYMBOL = 5;

    extern uint16_t SYSTEM_SYMBOLS = LAST_BASE_SYMBOL;
    extern uint16_t MAX_SYMBOLS = 0x7FFF; // MAX SYSTEM and USER SYMBOL
    extern uint16_t USER_SYMBOLS = 0xFFFF;
    extern const Symbol undefined_type("undefined", UNDEFINED_SYMBOL);
    extern const Symbol object_type("Object", OBJECT_SYMBOL);
    extern const Symbol array_type("Array", ARRAY_SYMBOL);
#endif

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
        // The ownership is handled in Object
        // it deletes unowned value there
        inline Value *operator*() { return get(); }
        inline Value *operator->() { return get(); }
        inline Value *get() { return _ptr; };
        inline Value &ref() { return *get(); }
    };

    struct Value : IVisitable
    {
        MAKE_VISITABLE(Value)
        Value() : _type(object_type) {}
        Value(const Symbol &type) : _type(type) {}
        virtual ~Value() {}
        const Symbol &_type;
        virtual std::shared_ptr<Value> make_shared() = 0;
        virtual ValuePtr clone() = 0;
    };

    // This is for defining metadata
    // a list of properties for a class
    struct Property
    {
        const Symbol &name;
        const Symbol &type;
        Value &value;
        bool owned = false;
    };

    // Metadata information
    struct Metadata
    {
        std::vector<Property> props;
    };

    struct UndefinedValue : Value
    {
        static UndefinedValue &UNDEFINED;
        UndefinedValue() : Value(undefined_type) {}
        virtual std::shared_ptr<Value> make_shared()
        {
            // This might be wrong
            return std::shared_ptr<Value>(new UndefinedValue(*this));
        }
        virtual ValuePtr clone()
        {
            // This might be wrong
            return ValuePtr(new UndefinedValue(*this), false);
        }
    };
#ifdef __DEFINE_STATIC__
    UndefinedValue &UndefinedValue::UNDEFINED = UndefinedValue();
#endif

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
        void accept(IVisitor *v)
        {
            IVisit<TValue> *visit_me = dynamic_cast<IVisit<TValue> *>(v);
            if (visit_me)
                visit_me->visit((TValue &)*this);
        }
    };

#ifndef __DEFINE_STATIC__
    extern const Symbol int_type;
    extern const Symbol int16_type;
    extern const Symbol float_type;
    extern const Symbol string_type;
#else
    extern const Symbol int_type("Int", INT_SYMBOL);
    extern const Symbol int16_type("Int16", INT_SYMBOL); // Types can have multiple names? Maybe useful for python
    extern const Symbol float_type("Float", FLOAT_SYMBOL);
    extern const Symbol string_type("String", STRING_SYMBOL);
#endif

    typedef TValue<int, int_type> Int;
    typedef TValue<float, float_type> Float;
    typedef TValue<std::string, string_type> String;
}

namespace std
{
    template <>
    struct hash<obj::Symbol>
    {
        std::size_t operator()(obj::Symbol const &s) const noexcept
        {
            return std::hash<std::uint16_t>{}(s._key);
        }
    };
    template <>
    struct hash<obj::ValuePtr>
    {
        std::size_t operator()(obj::ValuePtr const &s) const noexcept
        {
            return std::hash<size_t>{}((size_t)s._ptr);
        }
    };
}

namespace obj
{
    typedef std::unordered_set<Symbol> Keywords;
    typedef std::unordered_map<Symbol, ValuePtr> KeyValues;
    struct Object : Value
    {
        MAKE_VISITABLE_POLY(Object, Value)
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
        Object(const Metadata &meta)
        {
            for (auto p : meta.props)
            {
                if (p.owned)
                    set_owned(p.name, p.value);
                else
                    set(p.name, p.value);
            }
        }
        void init()
        {
            _kv = std::make_shared<KeyValues>();
            _kw = std::make_shared<Keywords>();
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
                    if (!i->second._owned)
                    {
                        delete i->second._ptr;
                    }
                }
            }
        }

        inline Object &set_owned(const Symbol &key, Value &value)
        {
            _kv->insert_or_assign(key, ValuePtr(&value, true));
            return *this;
        }

        // Map emplace is really slow
        // This make sure it only does this when appropriate
        inline Object &set(const Symbol &key, Value &value)
        {
            Value &v = get(key);
            if (v._type._key != value._type._key)
            {
                _kv->insert_or_assign(key, value.clone());
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
                _kv->insert_or_assign(key, value.clone());
            }

            return *this;
        }

        inline Value &get(const Symbol &key)
        {
            KeyValues::iterator v = _kv->find(key);
            if (v == _kv->end())
            {
                return UndefinedValue::UNDEFINED;
            }
            return v->second.ref();
        }
        inline Object &remove(const Symbol &key)
        {
            auto i = _kv->find(key);
            ValuePtr v = i->second;
            if (!v._owned)
                delete v._ptr;
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
        virtual std::shared_ptr<Value> make_shared()
        {
            return std::shared_ptr<Value>(new Object(*this));
        }
        virtual ValuePtr clone()
        {
            return ValuePtr(new Object(*this), false);
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

    typedef std::vector<ValuePtr> ValueArray;
    struct Array : Value
    {
        MAKE_VISITABLE(Array)
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
            if (_vec.use_count() < 2)
            {
                for (auto i = _vec->begin(); i != _vec->end(); i++)
                {
                    if (!i->_owned)
                    {
                        delete i->_ptr;
                    }
                }
            }
        }
        virtual std::shared_ptr<Value> make_shared()
        {
            return std::shared_ptr<Value>(new Array(*this));
        }
        virtual ValuePtr clone()
        {
            return ValuePtr(new Array(*this), false);
        }
        Array &push(Value &v)
        {
            _vec->push_back(v.clone());
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

    struct SystemSymbol : Symbol
    {
        SystemSymbol(const char *name, uint16_t key = 0) : Symbol(name, key)
        {
            if (key == 0)
            {
                _key = SYSTEM_SYMBOLS;
                SYSTEM_SYMBOLS = ++SYSTEM_SYMBOLS;
                if (SYSTEM_SYMBOLS > MAX_SYMBOLS)
                {
                    // PANIC
                }
            }
        }
    };
    typedef SystemSymbol PropertySymbol;
    // Symbols for PYTHON scripts?
    struct UserSymbol : Symbol
    {
        UserSymbol(const char *name, uint16_t key = 0) : Symbol(name, key)
        {
            if (key == 0)
            {
                _key = USER_SYMBOLS;
                USER_SYMBOLS = USER_SYMBOLS--;

                if (USER_SYMBOLS > MAX_SYMBOLS)
                {
                    // PANIC
                }
            }
        }
    };
#ifdef __DEFINE_STATIC__
    Symbol *Symbol::get_or_create_symbol(const char *name)
    {
        auto s = Symbol::symbols.find(name);
        if (s == Symbol::symbols.end())
        {
            // Currently construct adds
            return new UserSymbol(name);
        }
        else
        {
            return s->second;
        }
    }

#endif

} // namespace obj
