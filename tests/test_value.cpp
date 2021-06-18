#include "catch2/catch.hpp"
#include "value.h"
#include "base_types.h"

using namespace obj;

TEST_CASE( "Value", "[value]" ) {
    


    // SECTION( "Symbols get name and key" ) {
    //     REQUIRE( test._name == "Test1" );
    //     REQUIRE( test._key== 42 );
    // }

    
}

TEST_CASE( "ValuePtr", "[value]" ) {
    
    ValuePtr one(Int(1));
    ValuePtr another_one(Int(1));
    ValuePtr undef1(Undefined);
    ValuePtr undef2(Undefined);
    ValuePtr undef3(Undefined);
    ValuePtr int_sym(new Symbolic(int_type));


    

    SECTION( "Symbols get name and key" ) {
        REQUIRE( one.ref<Int>() == 1 );
        REQUIRE( another_one.ref<Int>() == 1 );
        REQUIRE( another_one.ref<Int>() ==  one.ref<Int>());

        REQUIRE( undef1.ref<Symbolic>() ==  undef2.ref<Symbolic>());
        REQUIRE( undef1.ref<Symbolic>() !=  int_sym.ref<Symbolic>());
    }

    
}