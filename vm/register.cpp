#include "shared/types.h"
#include "register.h"
#include "vm.h"
#include "register_math_tables.h"

using namespace wisp;

void Register::CopyValue(Register& rhs)
{
    this->m_value = rhs.m_value;
    this->m_type = rhs.m_type;
}

void Register::DestroyValue()
{
    m_value = 0;
    m_type = ValueType::None;
}

uint64 Register::GetPointerOffset()
{
    return m_value;
}

uint8* Register::GetPointerFromBase(uint8* base)
{
    return base + GetPointerOffset();
}

bool Register::GetBool()
{
    return m_value != 0u;
}

uint64 Register::GetNativeFunctionIndex()
{
    return m_value;
}

uint64 Register::GetTableIndex()
{
    return m_value;
}

uint64 Register::GetArrayIndex()
{
    return m_value;
}

uint64 Register::GetRawInteger()
{
    return m_value;
}

double Register::GetRawFP()
{
    return RetrieveValueFrom64BitInteger<double>();
}

//

void Register::SetInt8(int8 value)
{
    StoreValueIn64BitInteger(value);
    m_type = ValueType::Int8;
}

void Register::SetUInt8(uint8 value)
{
    StoreValueIn64BitInteger(value);
    m_type = ValueType::UInt8;
}

void Register::SetInt16(int16 value)
{
    StoreValueIn64BitInteger(value);
    m_type = ValueType::Int16;
}

void Register::SetUInt16(uint16 value)
{
    StoreValueIn64BitInteger(value);
    m_type = ValueType::UInt16;
}

void Register::SetInt32(int32 value)
{
    StoreValueIn64BitInteger(value);
    m_type = ValueType::Int32;
}

void Register::SetUInt32(uint32 value)
{
    StoreValueIn64BitInteger(value);
    m_type = ValueType::UInt32;
}

void Register::SetInt64(int64 value)
{
    StoreValueIn64BitInteger(value);
    m_type = ValueType::Int64;
}

void Register::SetUInt64(uint64 value)
{
    m_value = value;
    m_type = ValueType::UInt64;
}

void Register::SetPointer(uint64 offset)
{
    m_value = offset;
    m_type = ValueType::Pointer;
}

void Register::SetBool(bool value)
{
    m_value = (value) ? 1u : 0u;
    m_type = ValueType::Bool;
}

void Register::SetFloat(float fp)
{
    StoreValueIn64BitInteger(fp);
    m_type = ValueType::Float;
}

void Register::SetDouble(double db)
{
    StoreValueIn64BitInteger(db);
    m_type = ValueType::Double;
}

void Register::SetNativeFunctionIndex(uint64 functionIndex)
{
    m_value = functionIndex;
    m_type = ValueType::NativeFunction;
}

void Register::SetTableIndex(uint64 tableIndex)
{
    m_value = tableIndex;
    m_type = ValueType::Table;
}

void Register::SetArrayIndex(uint64 arrayIndex)
{
    m_value = arrayIndex;
    m_type = ValueType::Array;
}

void Register::SetRawInteger(uint64 value)
{
    m_value = value;
}

void Register::SetRawFP(double value)
{
    StoreValueIn64BitInteger(value);
}

VmError Register::AddUnsignedInteger(uint64 value)
{
    assert(IsUnsignedInteger());

    if (m_type == ValueType::UInt8)
    {
        SetInt8(GetInteger<uint8>() + static_cast<uint8>(value));
    }
    else if (m_type == ValueType::UInt16)
    {
        SetInt16(GetInteger<uint16>() + static_cast<uint16>(value));
    }
    else if (m_type == ValueType::UInt32)
    {
        SetInt32(GetInteger<uint32>() + static_cast<uint32>(value));
    }
    else if (m_type == ValueType::UInt64)
    {
        SetInt64(GetInteger<uint64>() + value);
    }
    else
    {
        return VmError::RegisterMismatch;
    }

    return VmError::OK;
}

VmError Register::AddSignedInteger(int64 value)
{
    assert(IsSignedInteger());

    if (m_type == ValueType::Int8)
    {
        SetInt8(GetInteger<int8>() + static_cast<int8>(value));
    }
    else if (m_type == ValueType::Int16)
    {
        SetInt16(GetInteger<int16>() + static_cast<int16>(value));
    }
    else if (m_type == ValueType::Int32)
    {
        SetInt32(GetInteger<int32>() + static_cast<int32>(value));
    }
    else if (m_type == ValueType::Int64)
    {
        SetInt64(GetInteger<int64>() + value);
    }
    else
    {
        return VmError::RegisterMismatch;
    }

    return VmError::OK;
}

VmError Register::AddFloatingPoint(double value)
{
    assert(IsFloatingPoint());

    if (m_type == ValueType::Float)
    {
        StoreValueIn64BitInteger(RetrieveValueFrom64BitInteger<float>() + static_cast<float>(value));
    }
    else if (m_type == ValueType::Double)
    {
        StoreValueIn64BitInteger(RetrieveValueFrom64BitInteger<double>() + value);
    }
    else
    {
        return VmError::RegisterMismatch;
    }

    return VmError::OK;
}

VmError Register::SubUnsignedInteger(uint64 value)
{
    assert(IsUnsignedInteger());

    if (m_type == ValueType::UInt8)
    {
        SetInt8(GetInteger<uint8>() - static_cast<uint8>(value));
    }
    else if (m_type == ValueType::UInt16)
    {
        SetInt16(GetInteger<uint16>() - static_cast<uint16>(value));
    }
    else if (m_type == ValueType::UInt32)
    {
        SetInt32(GetInteger<uint32>() - static_cast<uint32>(value));
    }
    else if (m_type == ValueType::UInt64)
    {
        SetInt64(GetInteger<uint64>() - value);
    }
    else
    {
        return VmError::RegisterMismatch;
    }

    return VmError::OK;
}

VmError Register::SubSignedInteger(int64 value)
{
    assert(IsSignedInteger());

    if (m_type == ValueType::Int8)
    {
        SetInt8(GetInteger<int8>() - static_cast<int8>(value));
    }
    else if (m_type == ValueType::Int16)
    {
        SetInt16(GetInteger<int16>() - static_cast<int16>(value));
    }
    else if (m_type == ValueType::Int32)
    {
        SetInt32(GetInteger<int32>() - static_cast<int32>(value));
    }
    else if (m_type == ValueType::Int64)
    {
        SetInt64(GetInteger<int64>() - value);
    }
    else
    {
        return VmError::RegisterMismatch;
    }

    return VmError::OK;
}

VmError Register::SubFloatingPoint(double value)
{
    assert(IsFloatingPoint());

    if (m_type == ValueType::Float)
    {
        StoreValueIn64BitInteger(RetrieveValueFrom64BitInteger<float>() - static_cast<float>(value));
    }
    else if (m_type == ValueType::Double)
    {
        StoreValueIn64BitInteger(RetrieveValueFrom64BitInteger<double>() - value);
    }
    else
    {
        return VmError::RegisterMismatch;
    }

    return VmError::OK;
}

VmError Register::AddRegister(Register& rhs)
{
	return AddRegisterOperationTable[static_cast<uint8>(GetType())][static_cast<uint8>(rhs.GetType())](*this, rhs);
}

VmError Register::SubRegister(Register& rhs)
{
	return SubRegisterOperationTable[static_cast<uint8>(GetType())][static_cast<uint8>(rhs.GetType())](*this, rhs);
}