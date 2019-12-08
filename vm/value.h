#pragma once

// Values are created when an instruction is being parsed based on the instruction prototype
// If there is a value mismatch with the instruction prototype, a failure occurs
// Registers also have a value pointer that is updated as parsing happens, as-needed
// Values are the lowest basic type in the virtual machine. They can be cast into other value types.

// Every integer value is treated as uint64 and cast into smaller values
// This makes the VM a little more simple, at the expense of memory. It is what it is,
// but shouldn't be hard to change.

// Function values in the VM don't really "exist"
// We know function values are function values based on the instruction prototype alone
// and we treat the value provided as a function

// Pointer values are just a value offset into the program image
// or, when allocation occurs, the function will return a pointer value and it will be acted upon
// depending on what instruction prototype it is fed into.

namespace wisp
{
    enum class ValueType
    {
        None = 0,
        Int8,
        UInt8,
        Int16,
        Uint16,
        Int32,
        UInt32,
        Int64,
        UInt64,
        Pointer,
        Bool,
        Float,
        Double,
        Function,
        Table,
        Array
    };

    class Value
    {
    public:
        virtual ~Value() = default;

    private:
    };

    class IntegerValue : public Value
    {
    public:
        IntegerValue() : m_value(0) {}
        IntegerValue(uint64 value) : m_value(value) {}

        template<typename T>
        T GetInt()
        {
            return static_cast<T>(m_value);
        }

        template<typename T>
        void SetInt(const T& value)
        {
            m_value = static_cast<uint64>(value);
        }

    private:
        uint64 m_value;
    };

    class PointerValue : public Value
    {
    public:
        PointerValue() = delete;
        PointerValue(uint32 offset) : m_offset(offset) {}

        uint32 GetOffset()
        {
            return m_offset;
        }

        void SetOffset(uint32 offset)
        {
            m_offset = offset;
        }

        void Increment(uint32 delta)
        {
            m_offset += delta;
        }

        void Decrement(uint32 delta)
        {
            m_offset -= delta;
        }

    private:
        uint32 m_offset;
    };

    class BooleanValue : public Value
    {
        //
    };

    class FloatValue : public Value
    {
        //
    };

    class DoubleValue : public Value
    {
        //
    };

    class FunctionValue : public Value
    {
        //
    };

    class NativeFunctionValue : public Value
    {
        //
    };

    class TableValue : public Value
    {
        //
    };

    class ArrayValue : public Value
    {
        //
    };
}
