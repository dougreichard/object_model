#include "catch2/catch.hpp"

#include "base_types.h"
#include "enum.h"

using namespace obj;

namespace Enum
{
// Symbol can be used as a string
// or as an uint16_t
    static const inline PropertySymbol EMP("EMP");
    static const inline PropertySymbol ATTRACT("Attract");
    static const inline PropertySymbol PSHOCK("PShock");
}; // namespace ES

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