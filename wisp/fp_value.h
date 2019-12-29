#pragma once

#include <cassert>
#include <variant>

namespace wisp
{
    enum class FPValueType
    {
        None = 0,
        Float,
        Double,
    };

    class FPValue
    {
    public:
        typedef std::variant<float, double> value_storage;

        FPValue() : m_hasValue(false), m_value(), m_size(0) {}
        FPValue(const value_storage& vs) : m_hasValue(true), m_value(vs), m_size(GetSizeFromIndex())
        {
        }

        bool HasValue() const
        {
            return m_hasValue;
        }

        bool IsFloat() const
        {
            return (GetType() == FPValueType::Float);
        }

        bool IsDouble() const
        {
            return (GetType() == FPValueType::Double);
        }

        // Setters
        void Clear()
        {
            m_hasValue = false;
            m_value = value_storage();
            m_size = 0;
        }

        template<typename T>
        void Set(T value)
        {
            m_hasValue = true;
            m_value = value;
            m_size = GetSizeFromIndex();
        }

        // Getters
        template<typename T>
        T Get()
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

        FPValueType GetType() const
        {
            if (!HasValue())
                return FPValueType::None;

            return static_cast<FPValueType>(m_value.index() + 1);
        }

        uint32 GetSize() const
        {
            return m_size;
        }

        friend FPValue operator+(const FPValue& a, const FPValue& b)
        {
            return a.Add(b);
        }

        friend FPValue operator-(const FPValue& a, const FPValue& b)
        {
            return a.Sub(b);
        }

        friend FPValue operator*(const FPValue& a, const FPValue& b)
        {
            return a.Mul(b);
        }

        friend FPValue operator/(const FPValue& a, const FPValue& b)
        {
            return a.Div(b);
        }

        friend FPValue operator+=(FPValue a, FPValue b)
        {
            a = a.Add(b);
        }

        friend FPValue operator-=(FPValue a, FPValue b)
        {
            a = a.Sub(b);
        }

        friend FPValue operator*=(FPValue a, FPValue b)
        {
            a = a.Mul(b);
            return a;
        }

        friend FPValue operator/=(FPValue a, FPValue b)
        {
            a = a.Div(b);
            return a;
        }

        FPValue Add(const FPValue& other) const;
        FPValue Sub(const FPValue& other) const;
        FPValue Mul(const FPValue& other) const;
        FPValue Div(const FPValue& other) const;

    private:
        uint32 GetSizeFromIndex()
        {
            if (!HasValue())
                return 0u;

            switch (static_cast<FPValueType>(m_value.index() + 1))
            {
            case FPValueType::Float:
                return sizeof(float);
            case FPValueType::Double:
                return sizeof(double);
            default:
                __assume(0);
            }
        }

        bool m_hasValue;
        value_storage m_value;
        uint32 m_size;
    };
}