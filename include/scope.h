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
        static inline std::unordered_map<std::string, TSymbol *> symbols;
        void add(const char *name, TSymbol *symbol)
        {
            Scope::instance().symbols.insert({name, symbol});
        }
        template <typename AddSymbol>
        TSymbol *get_or_create_symbol(const char *name)
        {
            auto all = Scope::instance();
            auto s = all.symbols.find(name);
            if (s == all.symbols.end())
            {
                // Currently construct adds
                return new AddSymbol(name);
            }
            else
            {
                return s->second;
            }
        }
        TSymbol& get(const char *name)
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
                return *s->second;
            }
        }
        
        static auto &instance()
        {
            static Scope scope;
            return scope;
        }
    };
}