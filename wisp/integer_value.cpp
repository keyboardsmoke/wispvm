#include "shared/types.h"
#include "integer_value.h"

using namespace wisp;

// IntegerValue Add(const IntegerValue& other);

IntegerValue IntegerValue::Add(const IntegerValue& other) const
{
	return std::visit([](auto&& arg1, auto&& arg2) -> value_storage { return arg1 + arg2; }, m_value, other.m_value);
}

IntegerValue IntegerValue::Sub(const IntegerValue& other) const
{
	return std::visit([](auto&& arg1, auto&& arg2) -> value_storage { return arg1 - arg2; }, m_value, other.m_value);
}

IntegerValue IntegerValue::Mul(const IntegerValue& other) const
{
	return std::visit([](auto&& arg1, auto&& arg2) -> value_storage { return arg1 * arg2; }, m_value, other.m_value);
}

IntegerValue IntegerValue::Div(const IntegerValue& other) const
{
	return std::visit([](auto&& arg1, auto&& arg2) -> value_storage { return arg1 / arg2; }, m_value, other.m_value);
}

IntegerValue IntegerValue::And(const IntegerValue& other) const
{
	return std::visit([](auto&& arg1, auto&& arg2) -> value_storage { return arg1 & arg2; }, m_value, other.m_value);
}

IntegerValue IntegerValue::Or(const IntegerValue& other) const
{
	return std::visit([](auto&& arg1, auto&& arg2) -> value_storage { return arg1 | arg2; }, m_value, other.m_value);
}

IntegerValue IntegerValue::Xor(const IntegerValue& other) const
{
	return std::visit([](auto&& arg1, auto&& arg2) -> value_storage { return arg1 ^ arg2; }, m_value, other.m_value);
}

IntegerValue IntegerValue::Shl(const IntegerValue& other) const
{
	return std::visit([](auto&& arg1, auto&& arg2) -> value_storage { return arg1 << arg2; }, m_value, other.m_value);
}

IntegerValue IntegerValue::Shr(const IntegerValue& other) const
{
	return std::visit([](auto&& arg1, auto&& arg2) -> value_storage { return arg1 >> arg2; }, m_value, other.m_value);
}

IntegerValue IntegerValue::Mod(const IntegerValue& other) const
{
	return std::visit([](auto&& arg1, auto&& arg2) -> value_storage { return arg1 % arg2; }, m_value, other.m_value);
}

void IntegerValue::UnaryPlus()
{
	m_value = std::visit([](auto&& arg) -> value_storage { return +arg; }, m_value);
}

void IntegerValue::UnaryMinus()
{
	assert(IsSignedInteger());

	m_value = std::visit([](auto&& arg) -> value_storage 
	{ 
		if (std::is_signed_v<decltype(arg)>) 
		{ 
			return -static_cast<signed>(arg); 
		}

		return 0; 
	}, m_value);
}

void IntegerValue::Not()
{
	m_value = std::visit([](auto&& arg) -> value_storage { return ~arg; }, m_value);
}

IntegerValue IntegerValue::PreIncrement()
{
	return std::visit([](auto&& arg) -> value_storage { return ++arg; }, m_value);
}

IntegerValue IntegerValue::PostIncrement()
{
	return std::visit([](auto&& arg) -> value_storage { return arg++; }, m_value);
}

IntegerValue IntegerValue::PreDecrement()
{
	return std::visit([](auto&& arg) -> value_storage { return --arg; }, m_value);
}

IntegerValue IntegerValue::PostDecrement()
{
	return std::visit([](auto&& arg) -> value_storage { return arg--; }, m_value);
}