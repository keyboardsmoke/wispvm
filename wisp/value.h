#pragma once

namespace wisp
{
    // There are exactly 15 types so that the ValueType can be encoded in 4 bits.
// We can't support more. If we need to save space for whatever reason,
// maybe types could be encoded as base type (int/real/native/table/array) and a width code.
    enum class ValueType
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

        // Boolean
        Bool,

        // FP represented
        Float,
        Double,

        // Native index represented
        NativeFunction,
        Table,
        Array,
        Max,
    };

    // Value is what Register will contain
    // It is an interface that allows rapid iteration
    class Value
    {
    public:
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
        friend Value& operator/=(Value& a, double b);

        Value Add(const Value& other);
        Value Sub(const Value& other);
        Value Mul(const Value& other);
        Value Div(const Value& other);

        Value And(const Value& other);
        Value Or(const Value& other);
        Value Xor(const Value& other);
        Value Shl(const Value& other);
        Value Shr(const Value& other);
        Value Mod(const Value& other);*/

    private:
        ValueType m_type;

        uint64 m_value;
    };
}