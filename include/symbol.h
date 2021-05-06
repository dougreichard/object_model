#pragma once
#include <string>
#include "scope.h"
#include <memory>

namespace obj
{
    ///////////////
    // These limits are vast
    static const inline uint64_t MAX_BASE_SYMBOLS  = 0x0FFFFFFFFFFFFFFF;
    static inline uint64_t SYSTEM_SYMBOLS  = 0x1000000000000000; 
    static inline uint64_t USER_SYMBOLS = 0xFFFFFFFFFFFFFFFF;
    static const inline int IS_USER = 1; 
    static const inline int IS_SYSTEM = 2; 
    // Human string, system int
    struct UnscopedSymbol
    {
        std::string _name;
        uint64_t _key;
        UnscopedSymbol(const char *name, uint64_t key) : _name(name), _key(key)
        {
        }
        operator const uint64_t &() const { return _key; }
        operator const std::string &() const { return _name; }
        static UnscopedSymbol &get_undefined()
        {
            static auto un = UnscopedSymbol("undefined", 0);
            return un;
        }
    };
    struct Symbol : UnscopedSymbol
    {
        Symbol(const char *name, uint64_t key) : UnscopedSymbol(name, key)
        {
            Scope<Symbol>::instance().add(name, this);
        }
        Symbol(const char *name, uint64_t key, int from) : UnscopedSymbol(name, key)
        {
            Scope<Symbol>::instance().add(name, this);
            if (key == 0 && from == IS_SYSTEM)
            {
                _key = SYSTEM_SYMBOLS;
                SYSTEM_SYMBOLS = ++SYSTEM_SYMBOLS;
                if (SYSTEM_SYMBOLS > USER_SYMBOLS)
                {
                    // PANIC The number of symbols overlapped
                    // this is extremely unlikely, it would take years
                }
            } else if (key == 0 && from == IS_USER)
            {
                _key = USER_SYMBOLS;
                USER_SYMBOLS = USER_SYMBOLS--;
                if (USER_SYMBOLS < SYSTEM_SYMBOLS)
                {
                    // PANIC The number of symbols overlapped
                    // this is extremely unlikely, it would take years
                }
            }
        }
    };
    struct SystemSymbol : Symbol
    {
        SystemSymbol(const char *name, uint64_t key = 0) : Symbol(name, key, IS_SYSTEM){}
    };
    typedef SystemSymbol PropertySymbol;
    // Symbols for PYTHON scripts?
    struct UserSymbol : Symbol
    {
        UserSymbol(const char *name, uint16_t key = 0) : Symbol(name, key, IS_USER)
        {
            
        }
    };

}
