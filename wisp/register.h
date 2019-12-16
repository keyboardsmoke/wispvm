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
#include <cstring>

#include "vm/vm.h"

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

        // Pointer
        Pointer,

        // Native index represented
        NativeFunction,
        Table,
        Array,
        Max,
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
            return RetrieveValueFrom64BitInteger<T>();
        }

        template<typename T>
        T GetFP()
        {
            return RetrieveValueFrom64BitInteger<T>();
        }

        uint64 GetPointerOffset();
        uint8* GetPointerFromBase(uint8 * base);
        bool GetBool();
        uint64 GetNativeFunctionIndex();
        uint64 GetTableIndex();
        uint64 GetArrayIndex();
        uint64 GetRawInteger();
        double GetRawFP();

        // Setters
        template<typename T>
        void SetInteger(T value, ValueType type)
        {
            m_type = type;
            assert(IsInteger());
            StoreValueIn64BitInteger<T>(value);
        }

        template<typename T>
        void SetFP(T value, ValueType type)
        {
            m_type = type;
            assert(IsFloatingPoint());
            StoreValueIn64BitInteger<T>(value);
        }

        void SetInt8(int8 value);
        void SetUInt8(uint8 value);
        void SetInt16(int16 value);
        void SetUInt16(uint16 value);
        void SetInt32(int32 value);
        void SetUInt32(uint32 value);
        void SetInt64(int64 value);
        void SetUInt64(uint64 value);
        void SetPointer(uint64 offset);
        void SetBool(bool value);
        void SetFloat(float fp);
        void SetDouble(double db);
        void SetNativeFunctionIndex(uint64 functionIndex);
        void SetTableIndex(uint64 tableIndex);
        void SetArrayIndex(uint64 arrayIndex);
        void SetRawInteger(uint64 value);
        void SetRawFP(double value);

        // Math operations
        vmcore::VmError AddUnsignedInteger(uint64 value);
        vmcore::VmError AddSignedInteger(int64 value);
        vmcore::VmError AddFloatingPoint(double value);

        vmcore::VmError SubUnsignedInteger(uint64 value);
        vmcore::VmError SubSignedInteger(int64 value);
        vmcore::VmError SubFloatingPoint(double value);

        // Basic Math
        vmcore::VmError AddRegister(Register & rhs);
        vmcore::VmError SubRegister(Register & rhs);
        vmcore::VmError MulRegister(Register & rhs);
        vmcore::VmError DivRegister(Register & rhs);
        vmcore::VmError ModuloRegister(Register & rhs);

        // Bitwise
        vmcore::VmError XorRegister(Register & rhs);
        vmcore::VmError OrRegister(Register & rhs);
        vmcore::VmError AndRegister(Register & rhs);
        vmcore::VmError LeftShiftRegister(Register & rhs);
        vmcore::VmError RightShiftRegister(Register & rhs);

        void CopyValue(Register & rhs);
        void DestroyValue();

    private:

        template<typename T>
        void StoreValueIn64BitInteger(const T & value)
        {
            memset(&m_value, 0, sizeof(uint64));
            memcpy(&m_value, &value, sizeof(T));
        }

        template<typename T>
        T RetrieveValueFrom64BitInteger()
        {
            T ret;
            memset(&ret, 0, sizeof(T));
            memcpy(&ret, &m_value, sizeof(T));
            return ret;
        }

        uint64 m_value;
        ValueType m_type;
    };
}