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

Value& ArrayValue::GetValue(size_t index)
{
    assert(index > 0 && index < m_vec.size());
    return m_vec[index];
}

void ArrayValue::SetValue(size_t index, Value value)
{
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