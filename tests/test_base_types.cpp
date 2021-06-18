#include "catch2/catch.hpp"

#include "base_types.h"

using namespace obj;

TEMPLATE_TEST_CASE("Common Base type value tests (Numbers)", "[base_types][template]",
                   Int, Int32, Int64, Float, Double, LongDouble)
{

    SECTION("default value")
    {
        TestType def;
        REQUIRE(def == 0);
    }
    SECTION("non default value construct")
    {
        TestType def(34);
        REQUIRE(def == 34);
    }
    SECTION("assignment")
    {
        TestType def;
        def = 56;
        REQUIRE(def == 56);
    }
}
TEST_CASE("Common Base type value tests (String)", "[base_types]")
{
    SECTION("default value")
    {
        String def;
        bool eq = def == "";
        REQUIRE(eq);
        eq = "" == def;
        REQUIRE(eq);
    }

    SECTION("non default value constructor")
    {
        String def("ThirtyFour");
        bool eq = def == "ThirtyFour";
        REQUIRE(eq);
        eq = "ThirtyFour" == def;
        REQUIRE(eq);
    }
    SECTION("assignment")
    {
        String def("Fifty6");
        bool eq = def == "Fifty6";
        REQUIRE(eq);
        eq = "Fifty6" == def;
        REQUIRE(eq);
    }
}


TEMPLATE_TEST_CASE("Common Base type clone tests ", "[base_types][template]",
    Int, Int32, Int64, Float, Double, LongDouble)
{
    TestType def(89);
    ValuePtr p(def.clone());
  //  TestType& get = *((TestType*)p.get());
    TestType& ref = ((TestType&)p.ref());

   // TestType* tget = p.get<TestType>();
    TestType& tref = p.ref<TestType>();

    SECTION("clone deref operator")
    {
        
        REQUIRE(def == 89);
        REQUIRE(ref == 89);
    //    REQUIRE(get == 89);
    //    REQUIRE(*tget == 89);
        REQUIRE(tref == 89);
    }

    // SECTION("clone ref")
    // {
        
    //     REQUIRE(def == 89);
    //     REQUIRE(p.ref() == 89);
    // }

}