#pragma once

#include "shared/types.h"
#include "integer_value.h"
#include "fp_value.h"
#include "complex_value.h"

namespace wisp
{
    enum class ValueType
    {
        None = 0,
        Integer,
        FP,
        String,
        Table,
        Array
    };

    class Value
    {
    public:
        typedef std::variant<IntegerValue, FPValue, StringValue, TableValue, ArrayValue> value_storage;

        Value() : m_hasValue(false), m_value() {}
        Value(const IntegerValue& value) : m_hasValue(true), m_value(value) {}
        Value(const FPValue& value) : m_hasValue(true), m_value(value) {}
        Value(const StringValue& value) : m_hasValue(true), m_value(value) {}
        Value(const TableValue& value) : m_hasValue(true), m_value(value) {}
        Value(const ArrayValue& value) : m_hasValue(true), m_value(value) {}

        bool HasValue() const
        {
            return m_hasValue;
        }

        bool IsIntegerValue() const
        {
            return (GetType() == ValueType::Integer);
        }

        bool IsFPValue() const
        {
            return (GetType() == ValueType::FP);
        }

        bool IsStringValue() const
        {
            return (GetType() == ValueType::String);
        }

        bool IsTableValue() const
        {
            return (GetType() == ValueType::Table);
        }

        bool IsArrayValue() const
        {
            return (GetType() == ValueType::Array);
        }

        ValueType GetType() const
        {
            if (!HasValue())
                return ValueType::None;

            return static_cast<ValueType>(m_value.index() + 1);
        }
        
        bool IsSameBaseType(Value& rhs);

        // Setters
        void Clear()
        {
            m_hasValue = false;
            m_value = value_storage();
        }

        template<typename T>
        void Set(T value)
        {
            m_hasValue = true;
            m_value = value;
        }

        // Getters
        template<typename T>
        T& Get()
        {
            assert(HasValue());
            T* pval = std::get_if<T>(&m_value);
            assert(pval != nullptr);
            return *pval;
        }

        value_storage GetValue() const
        {
            return m_value;
        }

    private:
        bool m_hasValue;
        value_storage m_value;
    };
}