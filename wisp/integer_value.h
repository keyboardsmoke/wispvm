#pragma once

#include <cassert>
#include <variant>

namespace wisp
{
    // There are exactly 15 types so that the ValueType can be encoded in 4 bits.
// We can't support more. If we need to save space for whatever reason,
// maybe types could be encoded as base type (int/real/native/table/array) and a width code.
    enum class IntegerValueType
    {
        None = 0,

        // Integer Represented (signed)
        Int8,
        Int16,
        Int32,
        Int64,

        // Integer Represented (unsigned)
        UInt8,
        UInt16,
        UInt32,
        UInt64,
    };

    // Value is what Register will contain
    // It is an interface that allows rapid iteration
    class IntegerValue
    {
    public:
        typedef std::variant<int8, int16, int32, int64, uint8, uint16, uint32, uint64> value_storage;

        IntegerValue() : m_value() {}
        IntegerValue(const value_storage& vs) : m_value(vs) {}

        bool HasValue()
        {
            return !(m_value.index() == std::variant_npos);
        }

        bool IsSignedInteger()
        {
            if (!HasValue())
                return false;

            std::size_t valueAvoidNone = m_value.index() + 1;
            uint32 intBegin = static_cast<uint32>(IntegerValueType::Int8);
            uint32 intEnd = static_cast<uint32>(IntegerValueType::Int64);
            return valueAvoidNone >= intBegin && valueAvoidNone <= intEnd;
        }

        bool IsUnsignedInteger()
        {
            if (!HasValue())
                return false;

            std::size_t valueAvoidNone = m_value.index() + 1;
            uint32 intBegin = static_cast<uint32>(IntegerValueType::UInt8);
            uint32 intEnd = static_cast<uint32>(IntegerValueType::UInt64);
            return valueAvoidNone >= intBegin && valueAvoidNone <= intEnd;
        }

        // Setters
        template<typename T>
        void Set(T value)
        {
            m_value = value;
        }

        // Getters
        template<typename T>
        T Get()
        {
            T* pval = std::get_if<T>(m_value);
            assert(pval != nullptr);
            return *pval;
        }

        IntegerValueType GetType()
        {
            if (!HasValue())
                return IntegerValueType::None;

            return static_cast<IntegerValueType>(m_value.index() + 1);
        }


        /*
        friend Value& operator+(Value& a, Value& b);
        friend Value& operator+(Value& a, int8 b);
        friend Value& operator+(Value& a, int16 b);
        friend Value& operator+(Value& a, int32 b);
        friend Value& operator+(Value& a, int64 b);
        friend Value& operator+(Value& a, uint8 b);
        friend Value& operator+(Value& a, uint16 b);
        friend Value& operator+(Value& a, uint32 b);
        friend Value& operator+(Value& a, uint64 b);
        friend Value& operator+(Value& a, float b);
        friend Value& operator+(Value& a, double b);

        friend Value& operator-(Value& a, Value& b);
        friend Value& operator-(Value& a, int8 b);
        friend Value& operator-(Value& a, int16 b);
        friend Value& operator-(Value& a, int32 b);
        friend Value& operator-(Value& a, int64 b);
        friend Value& operator-(Value& a, uint8 b);
        friend Value& operator-(Value& a, uint16 b);
        friend Value& operator-(Value& a, uint32 b);
        friend Value& operator-(Value& a, uint64 b);
        friend Value& operator-(Value& a, float b);
        friend Value& operator-(Value& a, double b);

        friend Value& operator*(Value& a, Value& b);
        friend Value& operator*(Value& a, int8 b);
        friend Value& operator*(Value& a, int16 b);
        friend Value& operator*(Value& a, int32 b);
        friend Value& operator*(Value& a, int64 b);
        friend Value& operator*(Value& a, uint8 b);
        friend Value& operator*(Value& a, uint16 b);
        friend Value& operator*(Value& a, uint32 b);
        friend Value& operator*(Value& a, uint64 b);
        friend Value& operator*(Value& a, float b);
        friend Value& operator*(Value& a, double b);

        friend Value& operator/(Value& a, Value& b);
        friend Value& operator/(Value& a, int8 b);
        friend Value& operator/(Value& a, int16 b);
        friend Value& operator/(Value& a, int32 b);
        friend Value& operator/(Value& a, int64 b);
        friend Value& operator/(Value& a, uint8 b);
        friend Value& operator/(Value& a, uint16 b);
        friend Value& operator/(Value& a, uint32 b);
        friend Value& operator/(Value& a, uint64 b);
        friend Value& operator/(Value& a, float b);
        friend Value& operator/(Value& a, double b);

        friend Value& operator+=(Value& a, Value& b);
        friend Value& operator+=(Value& a, int8 b);
        friend Value& operator+=(Value& a, int16 b);
        friend Value& operator+=(Value& a, int32 b);
        friend Value& operator+=(Value& a, int64 b);
        friend Value& operator+=(Value& a, uint8 b);
        friend Value& operator+=(Value& a, uint16 b);
        friend Value& operator+=(Value& a, uint32 b);
        friend Value& operator+=(Value& a, uint64 b);
        friend Value& operator+=(Value& a, float b);
        friend Value& operator+=(Value& a, double b);

        friend Value& operator-=(Value& a, Value& b);
        friend Value& operator-=(Value& a, int8 b);
        friend Value& operator-=(Value& a, int16 b);
        friend Value& operator-=(Value& a, int32 b);
        friend Value& operator-=(Value& a, int64 b);
        friend Value& operator-=(Value& a, uint8 b);
        friend Value& operator-=(Value& a, uint16 b);
        friend Value& operator-=(Value& a, uint32 b);
        friend Value& operator-=(Value& a, uint64 b);
        friend Value& operator-=(Value& a, float b);
        friend Value& operator-=(Value& a, double b);

        friend Value& operator*=(Value& a, Value& b);
        friend Value& operator*=(Value& a, int8 b);
        friend Value& operator*=(Value& a, int16 b);
        friend Value& operator*=(Value& a, int32 b);
        friend Value& operator*=(Value& a, int64 b);
        friend Value& operator*=(Value& a, uint8 b);
        friend Value& operator*=(Value& a, uint16 b);
        friend Value& operator*=(Value& a, uint32 b);
        friend Value& operator*=(Value& a, uint64 b);
        friend Value& operator*=(Value& a, float b);
        friend Value& operator*=(Value& a, double b);

        friend Value& operator/=(Value& a, Value& b);
        friend Value& operator/=(Value& a, int8 b);
        friend Value& operator/=(Value& a, int16 b);
        friend Value& operator/=(Value& a, int32 b);
        friend Value& operator/=(Value& a, int64 b);
        friend Value& operator/=(Value& a, uint8 b);
        friend Value& operator/=(Value& a, uint16 b);
        friend Value& operator/=(Value& a, uint32 b);
        friend Value& operator/=(Value& a, uint64 b);
        friend Value& operator/=(Value& a, float b);
        friend Value& operator/=(Value& a, double b);*/

        IntegerValue Add(const IntegerValue& other);
        IntegerValue Sub(const IntegerValue& other);
        IntegerValue Mul(const IntegerValue& other);
        IntegerValue Div(const IntegerValue& other);

        IntegerValue And(const IntegerValue& other);
        IntegerValue Or(const IntegerValue& other);
        IntegerValue Xor(const IntegerValue& other);
        IntegerValue Shl(const IntegerValue& other);
        IntegerValue Shr(const IntegerValue& other);
        IntegerValue Mod(const IntegerValue& other);

    private:
        value_storage m_value;
    };
}