#include "catch2/catch.hpp"

#include "base_types.h"
#include "metadata.h"

using namespace obj;

namespace Prop
{
// Symbol can be used as a string
// or as an uint16_t
    static const inline PropertySymbol X("X");
    static const inline PropertySymbol Y("Y");
    static const inline PropertySymbol R("R");
    static const inline PropertySymbol W("W");
    static const inline PropertySymbol H("H");

}; // namespace ES

TEST_CASE( "Metadata", "[metadata]" ) {
    const SystemSymbol MinRectSymbol("MinRect");
    const Metadata min_metadata {
    {
        {Prop::X, int_type, defaultInt, false},
        {Prop::Y, int_type, defaultInt, false},
        {Prop::W, int_type, defaultInt, false},
        {Prop::H, int_type, defaultInt, false},
    }};

    //  const Metadata min_metadata {
    // {
    //     {Prop::X, int_type, Int(4).make_shared(), false},
    //     {Prop::Y, int_type, defaultInt, false},
    //     {Prop::W, int_type, defaultInt, false},
    //     {Prop::H, int_type, defaultInt, false},
    // }};

    SECTION( "Symbols get name and key" ) {
        REQUIRE( min_metadata.props.size() == 4);
        // REQUIRE( test._key== 42 );
    }
}