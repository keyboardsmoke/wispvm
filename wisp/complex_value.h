#pragma once

#include "shared/types.h"
#include "integer_value.h"
#include "fp_value.h"
#include <cassert>

namespace wisp
{
    class Value;
    enum class ValueType;

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
        StringValue() = default;
        StringValue(const std::string str) : m_string(str) {}

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
        //    typedef std::variant<IntegerValue, FPValue, StringValue, TableValue, ArrayValue> value_storage;
        typedef std::variant<IntegerValueType, FPValueType, ComplexValueType> element_type;

        ArrayValue() = delete;
        ArrayValue(IntegerValueType elementType);
        ArrayValue(FPValueType elementType);
        ArrayValue(ComplexValueType elementType);

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
        ValueType GetElementValueType() const;
        element_type GetElementType() const;

    private:
        element_type m_type;
        ValueType m_valueType;
        std::vector<Value> m_vec;
    };

}