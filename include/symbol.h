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
        UnscopedSymbol() : _name("undefined"), _key(0) {}
        UnscopedSymbol(const char *name, uint64_t key) : _name(name), _key(key)
        {
        }
        UnscopedSymbol(const UnscopedSymbol& rhs) : _name(rhs._name), _key(rhs._key) {}
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
        Symbol()  : UnscopedSymbol() { }
        Symbol(const Symbol& rhs)  : UnscopedSymbol(rhs) {
            _name = rhs._name;
            _key = rhs._key;
        }
        
        Symbol(const char *name, uint64_t key) : UnscopedSymbol(name, key)
        {
            Scope<Symbol>::instance().add_or_fix_key(name, *this);
        }
        Symbol(const char *name, uint64_t key, int from) : UnscopedSymbol(name, key)
        {
            if (key == 0 && from == IS_SYSTEM)
            {
                _key = SYSTEM_SYMBOLS++;
                if (SYSTEM_SYMBOLS > USER_SYMBOLS)
                {
                    // PANIC The number of symbols overlapped
                    // this is extremely unlikely, it would take years
                }
            } else if (key == 0 && from == IS_USER)
            {
                _key = USER_SYMBOLS--;
                if (USER_SYMBOLS < SYSTEM_SYMBOLS)
                {
                    // PANIC The number of symbols overlapped
                    // this is extremely unlikely, it would take years
                }
            }
            Scope<Symbol>::instance().add_or_fix_key(name, *this);
        }
        void operator = (const Symbol &rhs ) {
            _name = rhs._name;
            _key = rhs._key;
        }
        bool operator == (const Symbol &rhs ) {
            //_name = rhs._name;
            return _key == rhs._key;
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
        UserSymbol()  : Symbol() { }
        UserSymbol(const UserSymbol& rhs) : Symbol(rhs) {
        }
        UserSymbol(const char *name, uint16_t key = 0) : Symbol(name, key, IS_USER)
        {
            
        }
    };
}


namespace std
{
    template <>
    struct hash<obj::Symbol>
    {
        std::size_t operator()(obj::Symbol const &s) const noexcept
        {
            return std::hash<std::size_t>{}(s._key);
        }
    };
}
