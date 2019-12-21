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

        bool HasValue()
        {
            return m_hasValue;
        }

        bool IsFloat()
        {
            return (GetType() == FPValueType::Float);
        }

        bool IsDouble()
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

        FPValueType GetType()
        {
            if (!HasValue())
                return FPValueType::None;

            return static_cast<FPValueType>(m_value.index() + 1);
        }

        uint32 GetSize() const
        {
            return m_size;
        }

        FPValue Add(const FPValue& other);
        FPValue Sub(const FPValue& other);
        FPValue Mul(const FPValue& other);
        FPValue Div(const FPValue& other);

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