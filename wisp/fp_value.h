#pragma once

#include <cassert>
#include <variant>

namespace wisp
{
    // There are exactly 15 types so that the ValueType can be encoded in 4 bits.
// We can't support more. If we need to save space for whatever reason,
// maybe types could be encoded as base type (int/real/native/table/array) and a width code.
    enum class FPValueType
    {
        None = 0,
        Float,
        Double,
    };

    // Value is what Register will contain
    // It is an interface that allows rapid iteration
    class FPValue
    {
    public:
        typedef std::variant<float, double> value_storage;

        FPValue() : m_value() {}
        FPValue(const value_storage& vs) : m_value(vs) {}

        bool HasValue()
        {
            return !(m_value.index() == std::variant_npos);
        }

        bool IsFloat()
        {
            if (!HasValue())
                return false;

            return (m_value.index() + 1) == static_cast<std::size_t>(FPValueType::Float);
        }

        bool IsDouble()
        {
            if (!HasValue())
                return false;

            return (m_value.index() + 1) == static_cast<std::size_t>(FPValueType::Double);
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

        FPValueType GetType()
        {
            if (!HasValue())
                return FPValueType::None;

            return static_cast<FPValueType>(m_value.index() + 1);
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

        FPValue Add(const FPValue& other);
        FPValue Sub(const FPValue& other);
        FPValue Mul(const FPValue& other);
        FPValue Div(const FPValue& other);

    private:
        value_storage m_value;
    };
}