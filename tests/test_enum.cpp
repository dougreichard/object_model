#include "catch2/catch.hpp"

#include "base_types.h"
#include "enum.h"

using namespace obj;

struct Torps : Enum<Torps> {
    static inline const VP Torp{"Torp",Int(0)};
    static inline const VP Emp{"Emp",Int(0x2)};
    static inline const VP PShock{"PShock",Int(0x4)};

};
 

// TEST_CASE( "Enum", "[enum]" ) {
//     const SystemSymbol MinEnum("Torpedo");
//     const EnumSet min_enum {
//     {
//         {Enum::EMP, Int(1)},
//         {Enum::ATTRACT, Int(2)},
//         {Enum::PSHOCK, Int(4)},
//         //{Enum::None, UndefinedValue()},
//     }};

//     SECTION( "Symbols get name and key" ) {
//         REQUIRE( min_enum.props.size() == 4);
//         // REQUIRE( test._key== 42 );
//     }
// }