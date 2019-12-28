#include "doctest/doctest.h"
#include "shared/types.h"
#include "wisp/integer_value.h"
#include "wisp/fp_value.h"
#include "wisp/complex_value.h"
#include "wisp/value.h"
#include <iostream>

using namespace wisp;

TEST_CASE("Type Equality")
{
    SUBCASE("Array base type")
    {
        Value a, b, c;
        a.Set(ArrayValue(FPValueType::Float));
        b.Set(ArrayValue(FPValueType::Float));
        c.Set(ArrayValue(FPValueType::Double));

        REQUIRE(a.IsSameBaseType(b));
        REQUIRE(a.IsSameBaseType(c) == false);
    }

    SUBCASE("Integer base type")
    {
        Value a, b, c;
        a.Set(IntegerValue(static_cast<uint32>(1)));
        b.Set(IntegerValue(static_cast<uint32>(2)));
        c.Set(IntegerValue(static_cast<int8>(1)));

        REQUIRE(a.IsSameBaseType(b));
        REQUIRE(a.IsSameBaseType(c) == false);
    }

    SUBCASE("Complex base type")
    {
        Value a, b;
        a.Set(StringValue("peep"));
        b.Set(ArrayValue(ComplexValueType::String));

        REQUIRE(a.IsSameBaseType(b) == false);
    }
}