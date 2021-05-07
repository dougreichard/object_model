#include "catch2/catch.hpp"
#include "symbol.h"

using namespace obj;

TEST_CASE( "Symbol", "[symbol]" ) {
    Symbol test ("Test1", 42);

    SECTION( "Symbols get name and key" ) {
        REQUIRE( test._name == "Test1" );
        REQUIRE( test._key== 42 );
    }

    
    SECTION( "Symbols with previous name gets previous key" ) {
        Symbol test2 ("Test1", 43);
        Symbol test3 ("Test1", 44);

        REQUIRE( test._name == "Test1" );
        REQUIRE( test._key== 42 );
        REQUIRE( test2._key== 42 );
        REQUIRE( test3._key== 42 );
    }
}



TEST_CASE( "User Symbols", "[symbol]" ) {
    //Scope<Symbol>::symbols.clear(); // clear symbols

    UserSymbol test1 ("Test1");
    UserSymbol test2 ("Test2");

        SECTION( "User symbols get non-0 key" ) {
        REQUIRE( test1._name == "Test1" );
        REQUIRE( test1._key!= 0 );
    }
   
    SECTION( "New User symbols get new key" ) {
        REQUIRE( test2._name == "Test2" );
        REQUIRE( test2._key!= 0 );
        REQUIRE( test2._key != test1._key );
    }
}

TEST_CASE( "System Symbols", "[symbol]" ) {
    //Scope<Symbol>::symbols.clear(); // clear symbols

    SystemSymbol test1 ("Test1");
    SystemSymbol test2 ("Test2");

    SECTION( "System symbols get non-0 key" ) {
        REQUIRE( test1._name == "Test1" );
        REQUIRE( test1._key!= 0 );
    }
   
    SECTION( "New System symbols get new key" ) {
        REQUIRE( test2._name == "Test2" );
        REQUIRE( test2._key!= 0 );
        REQUIRE( test2._key != test1._key );
    }
}

TEST_CASE( "Symbols are created in a scope", "[symbol]" ) {
    Scope<Symbol>::instance().symbols.clear(); // clear symbols

    Symbol test1 ("Test1", 1000);
    UserSymbol test2 ("Test2");
    SystemSymbol test3 ("Test3", 1002);

    Symbol& get1 = Scope<Symbol>::instance().get("Test1");
    Symbol& get2 = Scope<Symbol>::instance().get("Test2");
    Symbol& get3 = Scope<Symbol>::instance().get("Test3");

    REQUIRE( test1._name == get1._name );
    REQUIRE( test1._key == get1._key );
    REQUIRE( test2._name == get2._name );
    REQUIRE( test2._key== get2._key );
    REQUIRE( test3._name == get3._name );
    REQUIRE( test3._key== get3._key );
}

TEST_CASE( "Symbols created with get or create", "[symbol]" ) {
    Scope<Symbol>::instance().symbols.clear(); // clear symbols

    Symbol test1 = Scope<Symbol>::instance().get_or_create_symbol<UserSymbol>("Test1");
    Symbol test2 = Scope<Symbol>::instance().get_or_create_symbol<UserSymbol>("Test1");
    Symbol test3 = Scope<Symbol>::instance().get_or_create_symbol<UserSymbol>("Test1");
    
    REQUIRE( test1._name == "Test1" );
    REQUIRE( test2._name == "Test1" );
    REQUIRE( test3._name == "Test1" );
    REQUIRE( test1._key == test2._key );
    REQUIRE( test2._key == test3._key );
}