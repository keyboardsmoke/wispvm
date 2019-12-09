#pragma once

// Wisp operands are not like intermediate operands from <internal name redacted>
// They are for storing the active register state in the State structure
// They have no ID associated with them, the ID lookup will occur in the VM
// They are just here to store what's happening _now_ and nothing else.
// When an instruction is being parsed, we won't be creating a _new_ register object
// They will already exist in the State structure, and will be updated or read accordingly
// We will need to know what type of value they've been set to
// So they will take a "Value" type that can be cast into:
//      Native Functions
//      Functions
//      Integers
//      Floats
//      Doubles
//      Arrays
//      Etc.

#include <cassert>

namespace wisp
{
    enum class VmError;

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

        // Pointer
        Pointer,

        // Native index represented
        NativeFunction,
        Table,
        Array
    };

    class Register
    {
    public:
        Register() : m_value(0), m_type(ValueType::None) {}

        bool HasValue()
        {
            return (m_type != ValueType::None);
        }

        bool IsInteger()
        {
            uint32 typeNumber = static_cast<uint32>(m_type);
            uint32 intBegin = static_cast<uint32>(ValueType::Int8);
            uint32 intEnd = static_cast<uint32>(ValueType::UInt64);
            return typeNumber >= intBegin && typeNumber <= intEnd;
        }

        bool IsSignedInteger()
        {
            uint32 typeNumber = static_cast<uint32>(m_type);
            uint32 intBegin = static_cast<uint32>(ValueType::Int8);
            uint32 intEnd = static_cast<uint32>(ValueType::Int64);
            return typeNumber >= intBegin && typeNumber <= intEnd;
        }

        bool IsUnsignedInteger()
        {
            uint32 typeNumber = static_cast<uint32>(m_type);
            uint32 intBegin = static_cast<uint32>(ValueType::UInt8);
            uint32 intEnd = static_cast<uint32>(ValueType::UInt64);
            return typeNumber >= intBegin && typeNumber <= intEnd;
        }

        bool IsFloatingPoint()
        {
            uint32 typeNumber = static_cast<uint32>(m_type);
            uint32 fpBegin = static_cast<uint32>(ValueType::Float);
            uint32 fpEnd = static_cast<uint32>(ValueType::Double);
            return typeNumber >= fpBegin && typeNumber <= fpEnd;
        }

        bool IsNativeIndexRepresented()
        {
            uint32 typeNumber = static_cast<uint32>(m_type);
            uint32 niBegin = static_cast<uint32>(ValueType::NativeFunction);
            uint32 niEnd = static_cast<uint32>(ValueType::Array);
            return typeNumber >= niBegin && typeNumber <= niEnd;
        }

        bool IsPointer()
        {
            return m_type == ValueType::Pointer;
        }

        bool IsBoolean()
        {
            return m_type == ValueType::Bool;
        }

        // Getters
        ValueType GetType()
        {
            return m_type;
        }

        template<typename T>
        T GetInteger()
        {
            return static_cast<T>(m_value);
        }

        template<typename T>
        T GetFP()
        {
            return static_cast<T>(m_fp);
        }

        uint64 GetPointerOffset()
        {
            return m_value;
        }

        uint8* GetPointerFromBase(uint8* base)
        {
            return base + GetPointerOffset();
        }

        bool GetBool()
        {
            return m_value != 0u;
        }

        uint64 GetNativeFunctionIndex()
        {
            return m_value;
        }

        uint64 GetTableIndex()
        {
            return m_value;
        }

        uint64 GetArrayIndex()
        {
            return m_value;
        }

        uint64 GetRawInteger()
        {
            return m_value;
        }

        double GetRawFP()
        {
            return m_fp;
        }

        // Setters
        void SetInt8(int8 value)
        {
            m_value = static_cast<uint64>(value);
            m_type = ValueType::Int8;
        }

        void SetUInt8(uint8 value)
        {
            m_value = static_cast<uint64>(value);
            m_type = ValueType::UInt8;
        }

        void SetInt16(int16 value)
        {
            m_value = static_cast<uint64>(value);
            m_type = ValueType::Int16;
        }

        void SetUInt16(uint16 value)
        {
            m_value = static_cast<uint64>(value);
            m_type = ValueType::UInt16;
        }

        void SetInt32(int32 value)
        {
            m_value = static_cast<uint64>(value);
            m_type = ValueType::Int32;
        }

        void SetUInt32(uint32 value)
        {
            m_value = static_cast<uint64>(value);
            m_type = ValueType::UInt32;
        }

        void SetInt64(int64 value)
        {
            m_value = static_cast<uint64>(value);
            m_type = ValueType::Int64;
        }

        void SetUInt64(uint64 value)
        {
            m_value = value;
            m_type = ValueType::UInt64;
        }

        void SetPointer(uint64 offset)
        {
            m_value = offset;
            m_type = ValueType::Pointer;
        }

        void SetBool(bool value)
        {
            m_value = (value) ? 1u : 0u;
            m_type = ValueType::Bool;
        }

        void SetFloat(float fp)
        {
            m_fp = static_cast<double>(fp);
            m_type = ValueType::Float;
        }

        void SetDouble(double db)
        {
            m_fp = db;
            m_type = ValueType::Double;
        }

        void SetNativeFunctionIndex(uint64 functionIndex)
        {
            m_value = functionIndex;
            m_type = ValueType::NativeFunction;
        }

        void SetTableIndex(uint64 tableIndex)
        {
            m_value = tableIndex;
            m_type = ValueType::Table;
        }

        void SetArrayIndex(uint64 arrayIndex)
        {
            m_value = arrayIndex;
            m_type = ValueType::Array;
        }

        void SetRawInteger(uint64 value)
        {
            assert(IsInteger());
            m_value = value;
        }

        void SetRawFP(double value)
        {
            assert(IsFloatingPoint());
            m_fp = value;
        }

        // Math operations
        VmError AddUnsignedInteger(uint64 value);
        VmError AddSignedInteger(int64 value);
        VmError AddFloatingPoint(double value);

        VmError SubUnsignedInteger(uint64 value);
        VmError SubSignedInteger(int64 value);
        VmError SubFloatingPoint(double value);

        VmError AddRegister(Register& rhs);
        VmError SubRegister(Register& rhs);

        void CopyValue(Register& rhs);
        void DestroyValue();

    private:

        union
        {
            uint64 m_value;
            double m_fp;
        };

        ValueType m_type;
    };
}