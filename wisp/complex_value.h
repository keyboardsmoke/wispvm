#pragma once

#include "shared/types.h"
#include "integer_value.h"
#include "fp_value.h"
#include <cassert>

namespace wisp
{
    class Value;

    enum class ComplexValueType
    {
        None = 0,
        String,
        Table,
        Array
    };

    class StringValue
    {
    public:
        std::string& GetString() { return m_string; }

        ComplexValueType GetType()
        {
            return ComplexValueType::String;
        }

    private:
        std::string m_string;
    };

    class TableValue
    {
    public:
        Value& GetValue(const std::string& key)
        {
            return m_map[key];
        }

        void SetValue(const std::string& key, Value value);

        std::unordered_map<std::string, Value>& GetMap()
        {
            return m_map;
        }

        ComplexValueType GetType()
        {
            return ComplexValueType::Table;
        }

    private:
        std::unordered_map<std::string, Value> m_map;
    };

    class ArrayValue
    {
    public:
        Value& GetValue(size_t index)
        {
            assert(index > 0 && index < m_vec.size());
            return m_vec[index];
        }

        void SetValue(size_t index, Value value);

        ComplexValueType GetType()
        {
            return ComplexValueType::Array;
        }

    private:
        std::vector<Value> m_vec;
    };

}