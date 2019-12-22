#include "complex_value.h"
#include "value.h"

using namespace wisp;

void TableValue::SetValue(const std::string& key, Value value)
{
    m_map[key] = value;
}

void ArrayValue::SetValue(size_t index, Value value)
{
    m_vec[index] = value;
}