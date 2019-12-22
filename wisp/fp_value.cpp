#include "shared/types.h"
#include "fp_value.h"

using namespace wisp;

FPValue FPValue::Add(const FPValue& other) const
{
	return std::visit([](auto&& arg1, auto&& arg2) -> value_storage { return arg1 + arg2; }, m_value, other.m_value);
}

FPValue FPValue::Sub(const FPValue& other) const
{
	return std::visit([](auto&& arg1, auto&& arg2) -> value_storage { return arg1 - arg2; }, m_value, other.m_value);
}

FPValue FPValue::Mul(const FPValue& other) const
{
	return std::visit([](auto&& arg1, auto&& arg2) -> value_storage { return arg1 * arg2; }, m_value, other.m_value);
}

FPValue FPValue::Div(const FPValue& other) const
{
	return std::visit([](auto&& arg1, auto&& arg2) -> value_storage { return arg1 / arg2; }, m_value, other.m_value);
}