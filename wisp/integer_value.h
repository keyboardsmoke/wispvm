#pragma once

#include <cassert>
#include <variant>

namespace wisp
{
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

        IntegerValue() : m_hasValue(false), m_value(), m_size(0)
        {
            //
        }

        IntegerValue(const value_storage& vs) : m_hasValue(true), m_value(vs), m_size(GetSizeFromIndex()) {}

        bool HasValue() const
        {
            return m_hasValue;
        }

        bool IsSignedInteger() const
        {
            if (!HasValue())
                return false;

            uint32 currentType = static_cast<uint32>(GetType());
            uint32 intBegin = static_cast<uint32>(IntegerValueType::Int8);
            uint32 intEnd = static_cast<uint32>(IntegerValueType::Int64);
            return currentType >= intBegin && currentType <= intEnd;
        }

        bool IsUnsignedInteger() const
        {
            if (!HasValue())
                return false;

            uint32 currentType = static_cast<uint32>(GetType());
            uint32 intBegin = static_cast<uint32>(IntegerValueType::UInt8);
            uint32 intEnd = static_cast<uint32>(IntegerValueType::UInt64);
            return currentType >= intBegin && currentType <= intEnd;
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

        IntegerValueType GetType() const
        {
            if (!HasValue())
                return IntegerValueType::None;

            return static_cast<IntegerValueType>(m_value.index() + 1);
        }

        uint32 GetSize() const 
        {
            return m_size;
        }

        friend IntegerValue operator+(const IntegerValue& a, const IntegerValue& b)
        {
            return a.Add(b);
        }

        friend IntegerValue operator-(const IntegerValue& a, const IntegerValue& b)
        {
            return a.Sub(b);
        }

        friend IntegerValue operator*(const IntegerValue& a, const IntegerValue& b)
        {
            return a.Mul(b);
        }

        friend IntegerValue operator/(const IntegerValue& a, const IntegerValue& b)
        {
            return a.Div(b);
        }

        friend IntegerValue operator&(const IntegerValue& a, const IntegerValue& b)
        {
            return a.And(b);
        }

        friend IntegerValue operator|(const IntegerValue& a, const IntegerValue& b)
        {
            return a.Or(b);
        }

        friend IntegerValue operator^(const IntegerValue& a, const IntegerValue& b)
        {
            return a.Xor(b);
        }

        friend IntegerValue operator<<(const IntegerValue& a, const IntegerValue& b)
        {
            return a.Shl(b);
        }

        friend IntegerValue operator>>(const IntegerValue& a, const IntegerValue& b)
        {
            return a.Shr(b);
        }

        friend IntegerValue operator%(const IntegerValue& a, const IntegerValue& b)
        {
            return a.Mod(b);
        }

        friend IntegerValue operator+=(IntegerValue a, IntegerValue b)
        {
            a = a.Add(b);
        }

        friend IntegerValue operator-=(IntegerValue a, IntegerValue b)
        {
            a = a.Sub(b);
        }

        friend IntegerValue operator*=(IntegerValue a, IntegerValue b)
        {
            a = a.Mul(b);
            return a;
        }

        friend IntegerValue operator/=(IntegerValue a, IntegerValue b)
        {
            a = a.Div(b);
            return a;
        }

        IntegerValue operator~()
        {
            IntegerValue ret = *this;
            ret.Not();
            return ret;
        }

        IntegerValue Add(const IntegerValue& other) const;
        IntegerValue Sub(const IntegerValue& other) const;
        IntegerValue Mul(const IntegerValue& other) const;
        IntegerValue Div(const IntegerValue& other) const;

        IntegerValue And(const IntegerValue& other) const;
        IntegerValue Or(const IntegerValue& other) const;
        IntegerValue Xor(const IntegerValue& other) const;
        IntegerValue Shl(const IntegerValue& other) const;
        IntegerValue Shr(const IntegerValue& other) const;
        IntegerValue Mod(const IntegerValue& other) const;

        void UnaryPlus();
        void UnaryMinus();
        void Not();

        IntegerValue PreIncrement();
        IntegerValue PostIncrement();

        IntegerValue PreDecrement();
        IntegerValue PostDecrement();

    private:
        uint32 GetSizeFromIndex()
        {
            if (!HasValue())
                return 0u;

            switch (static_cast<IntegerValueType>(m_value.index() + 1))
            {
            case IntegerValueType::Int8:
            case IntegerValueType::UInt8:
                return sizeof(uint8);
            case IntegerValueType::Int16:
            case IntegerValueType::UInt16:
                return sizeof(uint16);
            case IntegerValueType::Int32:
            case IntegerValueType::UInt32:
                return sizeof(uint32);
            case IntegerValueType::Int64:
            case IntegerValueType::UInt64:
                return sizeof(uint64);
            default:
                __assume(0);
            }
        }
        
        bool m_hasValue;
        value_storage m_value;
        uint32 m_size;
    };
}