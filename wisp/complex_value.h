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
        void SetString(const std::string& str);
        bool IsEmpty() const;
        void Append(const std::string& app);
        ComplexValueType GetType() const;

    private:
        std::string m_string;
    };

    class TableValue
    {
    public:
        Value& GetValue(const std::string& key);
        void SetValue(const std::string& key, Value value);
        void Clear();
        bool IsEmpty() const;
        size_t GetSize() const;
        std::unordered_map<std::string, Value>& GetMap();
        ComplexValueType GetType() const;

    private:
        std::unordered_map<std::string, Value> m_map;
    };

    class ArrayValue
    {
    public:
        Value& GetValue(size_t index);
        void SetValue(size_t index, Value value);
        void Clear();
        bool IsEmpty() const;
        void Reserve(size_t size);
        void Push(const Value& val);
        void Pop();
        void Insert(size_t index, const Value& val);
        void Erase(size_t index);
        ComplexValueType GetType() const;

    private:
        std::vector<Value> m_vec;
    };

}