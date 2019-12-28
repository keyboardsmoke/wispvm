#include "complex_value.h"
#include "value.h"

using namespace wisp;

void StringValue::SetString(const std::string& str)
{
    m_string = str;
}

bool StringValue::IsEmpty() const
{
    return m_string.empty();
}

void StringValue::Append(const std::string& app)
{
    m_string += app;
}

ComplexValueType StringValue::GetType() const
{
    return ComplexValueType::String;
}

//

Value& TableValue::GetValue(const std::string& key)
{
    return m_map[key];
}

void TableValue::SetValue(const std::string& key, Value value)
{
    m_map[key] = value;
}

void TableValue::Clear()
{
    m_map.clear();
}

bool TableValue::IsEmpty() const
{
    return m_map.empty();
}

size_t TableValue::GetSize() const
{
    return m_map.size();
}

std::unordered_map<std::string, Value>& TableValue::GetMap()
{
    return m_map;
}

ComplexValueType TableValue::GetType() const
{
    return ComplexValueType::Table;
}

//

static constexpr ValueType GetValueTypeFromComplexValueType(ComplexValueType type)
{
    switch (type)
    {
    case ComplexValueType::String:
        return ValueType::String;
    case ComplexValueType::Table:
        return ValueType::Table;
    case ComplexValueType::Array:
        return ValueType::Array;
    }

    return ValueType::None;
}

ArrayValue::ArrayValue(IntegerValueType elementType) : m_valueType(ValueType::Integer), m_type(elementType) {}
ArrayValue::ArrayValue(FPValueType elementType) : m_valueType(ValueType::FP), m_type(elementType) {}
ArrayValue::ArrayValue(ComplexValueType elementType) : m_valueType(GetValueTypeFromComplexValueType(elementType)), m_type(elementType) {}

Value& ArrayValue::GetValue(size_t index)
{
    assert(index > 0 && index < m_vec.size());
    return m_vec[index];
}

void ArrayValue::SetValue(size_t index, Value value)
{
    assert(index < m_vec.size());
    assert(m_vec[index].GetType() == value.GetType());

    // This works great when data already exists...
    // But, we still need to ensure our element type is equal to Value's GetValue type...
    assert(m_vec[index].IsSameBaseType(value));

    m_vec[index] = value;
}

void ArrayValue::Clear()
{
    m_vec.clear();
}

bool ArrayValue::IsEmpty() const
{
    return m_vec.empty();
}

void ArrayValue::Reserve(size_t size)
{
    m_vec.reserve(size);
}

void ArrayValue::Push(const Value& val)
{
    assert(std::visit([](auto&& a, auto&& b) -> bool
    {
        return std::is_same<decltype(a), decltype(b)>::value;
    }, GetElementType(), val.GetValue()));

    // assert(m_vec[index].GetType() == value.GetType());
    // assert(m_vec[index].IsSameBaseType(value));

    m_vec.push_back(val);
}

void ArrayValue::Pop()
{
    m_vec.pop_back();
}

void ArrayValue::Insert(size_t index, const Value& val)
{
    m_vec.insert(m_vec.begin() + index, val);
}

void ArrayValue::Erase(size_t index)
{
    m_vec.erase(m_vec.begin() + index);
}

ComplexValueType ArrayValue::GetType() const
{
    return ComplexValueType::Array;
}

ValueType ArrayValue::GetElementValueType() const
{
    return m_valueType;
}

ArrayValue::element_type ArrayValue::GetElementType() const
{
    return m_type;
}