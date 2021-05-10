#pragma once
#include "tvalue.h"

namespace obj
{

    static const inline uint64_t UNDEFINED_SYMBOL = 0;
    static const inline uint64_t OBJECT_SYMBOL = 1;
    static const inline uint64_t INT_SYMBOL = 2;
    static const inline uint64_t LONG_SYMBOL = 3;
    static const inline uint64_t LONG_LONG_SYMBOL = 4;
    static const inline uint64_t FLOAT_SYMBOL = 5;
    static const inline uint64_t DOUBLE_SYMBOL = 6;
    static const inline uint64_t LONG_DOUBLE_SYMBOL = 7;
    static const inline uint64_t STRING_SYMBOL = 8;
    static const inline uint64_t ARRAY_SYMBOL = 9;
    static const inline uint64_t VALUE_AS_SYMBOL = 10;
    static const inline uint64_t LAST_BASE_SYMBOL = 10;

    static const inline Symbol undefined_type("undefined", UNDEFINED_SYMBOL);
    static const inline Symbol object_type("Object", OBJECT_SYMBOL);
    static const inline Symbol array_type("Array", ARRAY_SYMBOL);

    static const inline Symbol int_type("Int", INT_SYMBOL);
    static const inline Symbol int16_type("Int16", INT_SYMBOL); 
    static const inline Symbol long_type("Long", LONG_SYMBOL);
    static const inline Symbol int32_type("Int32", LONG_SYMBOL);
    static const inline Symbol long_long_type("LongLong", LONG_LONG_SYMBOL);
    static const inline Symbol int64_type("Int64", LONG_LONG_SYMBOL);
    
    static const inline Symbol float_type("Float", FLOAT_SYMBOL);
    static const inline Symbol double_type("Double", DOUBLE_SYMBOL);
    static const inline Symbol long_double_type("LongDouble", DOUBLE_SYMBOL);

    static const inline Symbol string_type("String", STRING_SYMBOL);
    static const inline Symbol symbolic_type("Symbolic", VALUE_AS_SYMBOL);
    

    typedef TValue<int_fast16_t, int_type> Int;
    typedef TValue<int32_t, int32_type> Int32;
    typedef TValue<int64_t, int64_type> Int64;

    typedef TValue<float, float_type> Float;
    typedef TValue<double, double_type> Double;
    typedef TValue<long double, long_double_type> LongDouble;
    
    typedef TValue<std::string, string_type> String;
    typedef TValue<Symbol, symbolic_type> Symbolic;
    inline bool operator== (const String &c1, const char* c2)
    {
        return (c1._value == c2);
    }
    inline bool operator== ( const char* c2, const String &c1)
    {
        return (c1._value == c2);
    }
    inline bool operator== (const String &c1, const String& c2)
    {
        return (c1._value == c2._value);
    }

    struct UndefinedValue : Value
    {
        UndefinedValue() : Value(undefined_type) {}
        static UndefinedValue & get() {
            static UndefinedValue  instance;
            return instance;    
        }
        // virtual std::shared_ptr<Value> make_shared()
        // {
        //     return std::shared_ptr<Value>(&UndefinedValue::get());
        // }
        virtual Value* clone()
        {
            return &UndefinedValue::get();
        }
    };

    static const inline ValuePtr defaultInt(new Int(0), true);
    static const inline ValuePtr defaultInt32(new Int32(0), true);
    static const inline ValuePtr defaultInt64(new Int64(0), true);
    static const inline ValuePtr defaultFloat(new Float(0), true);
    static const inline ValuePtr defaultDouble(new Double(0), true);
    static const inline ValuePtr defaultLongDouble(new LongDouble(0), true);
    static const inline ValuePtr defaultString(new String(""), true);
}