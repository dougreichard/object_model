#pragma once
#include <unordered_map>

namespace obj
{
    ///////////////////////////////////////////////////////
    // Create a scope for Symbols
    // The main symbols scope and enums have a symbol scope
    template <typename TSymbol>
    struct Scope
    {
        std::unordered_map<std::string, TSymbol> symbols;
        void add_or_fix_key(const char *name, TSymbol& symbol)
        {
            // Add the symbol, if it exists, fix the key of the target
            auto i = Scope::instance().symbols.find( symbol._name );
            if (i != Scope::instance().symbols.end()) {
                 symbol._key = i->second._key;
            } else {
                 Scope::instance().symbols.emplace(symbol._name,  symbol);
            }
            
            
            
        }
        template <typename AddSymbol>
        TSymbol get_or_create_symbol(const char *name)
        {
            auto s = symbols.find(name);
            if (s == symbols.end())
            {
                TSymbol test = AddSymbol(name); //, 0, IS_USER);
                // Currently construct adds
                s = symbols.find(name);
            }
            return s->second;
            
        }
        TSymbol get(const char *name)
        {
            auto all = Scope::instance();
            auto s = all.symbols.find(name);
            if (s == all.symbols.end())
            {
                // Currently construct adds
                return (TSymbol&)TSymbol::get_undefined();
            }
            else
            {
                return s->second;
            }
        }
        
        static auto &instance()
        {
            static Scope scope;
            return scope;
        }
    };
}