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
    #define MAKE_VISITABLE(T) \
        void accept(IVisitor *v) { \
            IVisit<T> *visit_me = dynamic_cast<IVisit<T> *>(v); \
            if (visit_me) visit_me->visit((T&)*this); }
    #define MAKE_VISITABLE_POLY(T, B) \
        void accept(IVisitor *v) { \
            IVisit<T> *visit_me = dynamic_cast<IVisit<T> *>(v); \
            if (visit_me) visit_me->visit((T&)*this); else B::accept(v);}

    // Used to build visitors
    template <typename T>
    struct IVisit
    {
        virtual void visit(T &obj) = 0;
    };
    // Human string, system int
    struct Symbol
    {
        std::string _name;
        uint16_t _key;
        Symbol(const char *name, uint16_t key) : _name(name), _key(key)
        {
        }
        operator const uint16_t&() const { return _key; }
        operator const std::string &() const { return _name; }
    };
    struct Value;

    uint16_t UNDEFINED_SYMBOL = 0;
    uint16_t OBJECT_SYMBOL = 1;
    uint16_t INT_SYMBOL = 2;
    uint16_t FLOAT_SYMBOL = 3;
    uint16_t STRING_SYMBOL = 4;
    uint16_t ARRAY_SYMBOL = 5;
    uint16_t LAST_BASE_SYMBOL = 5;

    uint16_t SYSTEM_SYMBOLS = LAST_BASE_SYMBOL;
    uint16_t MAX_SYMBOLS = 0x7FFF; // MAX SYSTEM and USER SYMBOL
    uint16_t USER_SYMBOLS = 0xFFFF;

    Symbol undefined_type("undefined", UNDEFINED_SYMBOL);
    Symbol object_type("Object", OBJECT_SYMBOL);
    struct Value : IVisitable
    {
        MAKE_VISITABLE(Value)
        Value() : _type(object_type) {}
        Value(const Symbol &type) : _type(type) {}
        const Symbol &_type;
        virtual std::shared_ptr<Value> make_shared() = 0;
        virtual std::unique_ptr<Value> clone()=0;
    };

    // struct UndefinedValue : Value
    // {
    //     static UndefinedValue& UNDEFINED;
    //     UndefinedValue() : Value(undefined_type) {}
    //     virtual std::shared_ptr<Value>& make_shared() {
    //         return std::make_shared<Value>(UndefinedValue());
    //     }
    // };
    // UndefinedValue& UndefinedValue::UNDEFINED = UndefinedValue();

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
        virtual std::unique_ptr<Value> clone(){
            return std::unique_ptr<Value>(new TValue(*this));
        }
        void accept(IVisitor *v)
        {
            IVisit<TValue> *visit_me = dynamic_cast<IVisit<TValue> *>(v);
            if (visit_me)
                visit_me->visit((TValue &)*this);
        }
    };

    const Symbol int_type("Int", INT_SYMBOL);
    Symbol int16_type("Int16", INT_SYMBOL); // Types can have multiple names? Maybe useful for python
    Symbol float_type("Float", FLOAT_SYMBOL);
    Symbol string_type("String", STRING_SYMBOL);

    typedef TValue<int, int_type> Int;
    typedef TValue<float, float_type> Float;
    typedef TValue<std::string, string_type> String;
}

namespace std
{
    template<> struct hash<obj::Symbol>
    {
        std::size_t operator()(obj::Symbol const& s) const noexcept
        {
            return std::hash<std::uint16_t>{}(s._key);
        }
    };
}

namespace obj {
    typedef std::unordered_set<Symbol> Keywords;
    typedef std::unordered_map<Symbol, std::unique_ptr<Value>> KeyValues;
    struct Object : Value
    {
        MAKE_VISITABLE_POLY(Object, Value)
        std::shared_ptr<KeyValues> _kv;
        std::shared_ptr<Keywords> _kw;
        Object() : Value(object_type)
        {
            init();
        }
        Object(const Symbol& type) : Value(type)
        {
            init();
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
        inline Object &set(const Symbol &key, Value &value)
        {
            _kv->emplace(key, value.clone());
            return *this;
        }
        
        inline Value &get(const Symbol &key)
        {
            KeyValues::iterator v = _kv->find(key);
            if (v == _kv->end())
                return *v->second;
            ; //UndefinedValue::UNDEFINED;
            return *v->second;
        }
        inline Object &remove(const Symbol &key)
        {
            _kv->erase(key);
            return *this;
        }
        inline bool has_value(const Symbol &key)
        {
            return _kv->find(key) != _kv->end();
        }

        inline void add_role(const Symbol& role) {
            _kw->insert(role);
        }
        inline void remove_role(const Symbol& role) {
            _kw->erase(role);
        }
        inline bool has_role(const Symbol& role) {
            return _kw->find(role) != _kw->end();
        }
        virtual std::shared_ptr<Value> make_shared()
        {
            return std::shared_ptr<Value>(new Object(*this));
        }
        virtual std::unique_ptr<Value> clone(){
            return std::unique_ptr<Value>(new Object(*this));
        }
    };

    Symbol array_type("Array", OBJECT_SYMBOL);
    typedef std::vector<std::unique_ptr<Value>> ValueArray;
    struct Array : Value {
        MAKE_VISITABLE(Array)
        std::shared_ptr<ValueArray> _vec;
        Array() : Value(array_type)
        {
            _vec = std::make_shared<ValueArray>();
        }
        // Copy constructor all copies share the same key/values
        Array(const Array&rhs) : Value(rhs._type)
        {
            _vec = rhs._vec;
        }
        virtual std::shared_ptr<Value> make_shared()
        {
            return std::shared_ptr<Value>(new Array(*this));
        }
        virtual std::unique_ptr<Value> clone(){
            return std::unique_ptr<Value>(new Array(*this));
        }
        Array& push(Value& v) {
            _vec->push_back(v.clone());
            return *this;
        }
        inline Value & operator[](size_t idx) {
            return *_vec->at(idx);
        }
        inline const size_t size() const {
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

} // namespace obj
