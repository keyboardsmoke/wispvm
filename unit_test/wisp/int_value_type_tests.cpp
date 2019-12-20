#include "doctest/doctest.h"
#include "shared/types.h"
#include "wisp/integer_value.h"
#include <iostream>

template<class V>
std::type_info const& GetVariantType(V const& v)
{
	return std::visit([](auto&& x)->decltype(auto) { return typeid(x); }, v);
}

TEST_CASE("Integer Values")
{
	SUBCASE("Easy math")
	{
		
		uint8 n1 = 0xfe;
		int8 n2 = 2;

		wisp::IntegerValue a;
		a.Set(n1);

		wisp::IntegerValue b;
		b.Set(n2);

		// This gets promoted to signed int
		auto r = n1 + n2;

		// This gets promoted to signed int
		wisp::IntegerValue c = a + b;

		REQUIRE(typeid(r) == GetVariantType(c.GetValue()));
		REQUIRE(c.Get<decltype(r)>() == r);
	}

	SUBCASE("Unsigned Math Promotion")
	{
		uint32 a = 0xffffff;
		uint32 b = 0x123;

		wisp::IntegerValue i1 = a;
		wisp::IntegerValue i2 = b;

		auto r = a ^ b;
		wisp::IntegerValue ir = i1.Xor(i2);

		std::cout << "Type = " << GetVariantType(ir.GetValue()).name() << std::endl;

		REQUIRE(typeid(r) == GetVariantType(ir.GetValue()));
		REQUIRE(ir.Get<decltype(r)>() == r);
	}
}