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
    static const inline uint64_t STRING_SYMBOL = 6;
    static const inline uint64_t ARRAY_SYMBOL = 7;
    static const inline uint64_t VALUE_AS_SYMBOL = 8;
    static const inline uint64_t LAST_BASE_SYMBOL = 8;

    
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
    static const inline Symbol string_type("String", STRING_SYMBOL);
    static const inline Symbol symbolic_type("Symbolic", VALUE_AS_SYMBOL);
    

    typedef TValue<int_fast16_t, int_type> Int;
    typedef TValue<int32_t, int32_type> Int32;
    typedef TValue<int64_t, int64_type> Int64;

    typedef TValue<float, float_type> Float;
    typedef TValue<std::string, string_type> String;
    typedef TValue<Symbol, symbolic_type> Symbolic;


    struct UndefinedValue : Value
    {
        UndefinedValue() : Value(undefined_type) {}
        static UndefinedValue & get() {
            static UndefinedValue  instance;
            return instance;    
        }
        virtual std::shared_ptr<Value> make_shared()
        {
            return std::shared_ptr<Value>(&UndefinedValue::get());
        }
        virtual ValuePtr clone()
        {
            return ValuePtr(&UndefinedValue::get(), false);
        }
    };



    // This may be a memory leak
    extern const inline ValuePtr defaultInt(new Int(0), true);
    extern const inline ValuePtr defaultFloat(new Float(0), true);
    extern const inline ValuePtr defaultString(new String(""), true);

}