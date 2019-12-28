#include "shared/types.h"
#include "integer_value.h"
#include "fp_value.h"
#include "complex_value.h"
#include "value.h"
#include <iostream>

using namespace wisp;

template<typename T1, typename T2>
class TypeResolver
{
public:
    static bool IsSame(T1& a, T2& b);
};

template<typename T1, typename T2>
bool TypeResolver<T1, T2>::IsSame(T1& a, T2& b)
{
    UNREFERENCED_PARAMETER(a);
    UNREFERENCED_PARAMETER(b);

    return false;
}

template<>
bool TypeResolver<IntegerValue, IntegerValue>::IsSame(IntegerValue& a, IntegerValue& b)
{
    return a.GetType() == b.GetType();
}

template<>
bool TypeResolver<FPValue, FPValue>::IsSame(FPValue& a, FPValue& b)
{
    return a.GetType() == b.GetType();
}

template<>
bool TypeResolver<StringValue, StringValue>::IsSame(StringValue& a, StringValue& b)
{
    UNREFERENCED_PARAMETER(a);
    UNREFERENCED_PARAMETER(b);

    return true;
}

template<>
bool TypeResolver<TableValue, TableValue>::IsSame(TableValue& a, TableValue& b)
{
    // Table's dont really have an element type
    // It's designed to take all sorts of elements
    // So typeof(table1) == typeof(table2) regardless of content
    // Unlike array.
    UNREFERENCED_PARAMETER(a);
    UNREFERENCED_PARAMETER(b);

    return true;
}

template<>
bool TypeResolver<IntegerValueType, IntegerValueType>::IsSame(IntegerValueType& a, IntegerValueType& b)
{
    return a == b;
}

template<>
bool TypeResolver<FPValueType, FPValueType>::IsSame(FPValueType& a, FPValueType& b)
{
    return a == b;
}

template<>
bool TypeResolver<ComplexValueType, ComplexValueType>::IsSame(ComplexValueType& a, ComplexValueType& b)
{
    return a == b;
}

template<>
bool TypeResolver<ArrayValue, ArrayValue>::IsSame(ArrayValue& a, ArrayValue& b)
{
    return std::visit([](auto&& a, auto&& b) -> bool
    {
        return TypeResolver<std::remove_reference<decltype(a)>::type, std::remove_reference<decltype(b)>::type>::IsSame(a, b);
    }, a.GetElementType(), b.GetElementType());
}

bool Value::IsSameBaseType(Value& rhs)
{
    // Only compare if we both contain values..
    if ((HasValue() && rhs.HasValue()) && (GetType() == rhs.GetType()))
    {
        return std::visit([](auto&& a, auto&& b) -> bool
        {
            return TypeResolver<std::remove_reference<decltype(a)>::type, std::remove_reference<decltype(b)>::type>::IsSame(a, b);
        }, m_value, rhs.m_value);
    }

    return false;
}