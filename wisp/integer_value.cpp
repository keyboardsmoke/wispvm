#include "shared/types.h"
#include "integer_value.h"

using namespace wisp;

IntegerValue IntegerValue::Add(const IntegerValue& other)
{
	return std::visit([](auto&& arg1, auto&& arg2) -> value_storage { return arg1 + arg2; }, m_value, other.m_value);
}

IntegerValue IntegerValue::Sub(const IntegerValue& other)
{
	return std::visit([](auto&& arg1, auto&& arg2) -> value_storage { return arg1 - arg2; }, m_value, other.m_value);
}

IntegerValue IntegerValue::Mul(const IntegerValue& other)
{
	return std::visit([](auto&& arg1, auto&& arg2) -> value_storage { return arg1 * arg2; }, m_value, other.m_value);
}

IntegerValue IntegerValue::Div(const IntegerValue& other)
{
	return std::visit([](auto&& arg1, auto&& arg2) -> value_storage { return arg1 / arg2; }, m_value, other.m_value);
}

IntegerValue IntegerValue::And(const IntegerValue& other)
{
	return std::visit([](auto&& arg1, auto&& arg2) -> value_storage { return arg1 & arg2; }, m_value, other.m_value);
}

IntegerValue IntegerValue::Or(const IntegerValue& other)
{
	return std::visit([](auto&& arg1, auto&& arg2) -> value_storage { return arg1 | arg2; }, m_value, other.m_value);
}

IntegerValue IntegerValue::Xor(const IntegerValue& other)
{
	return std::visit([](auto&& arg1, auto&& arg2) -> value_storage { return arg1 ^ arg2; }, m_value, other.m_value);
}

IntegerValue IntegerValue::Shl(const IntegerValue& other)
{
	return std::visit([](auto&& arg1, auto&& arg2) -> value_storage { return arg1 << arg2; }, m_value, other.m_value);
}

IntegerValue IntegerValue::Shr(const IntegerValue& other)
{
	return std::visit([](auto&& arg1, auto&& arg2) -> value_storage { return arg1 >> arg2; }, m_value, other.m_value);
}

IntegerValue IntegerValue::Mod(const IntegerValue& other)
{
	return std::visit([](auto&& arg1, auto&& arg2) -> value_storage { return arg1 % arg2; }, m_value, other.m_value);
}