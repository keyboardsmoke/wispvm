#include "shared/types.h"
#include "register.h"
#include "vm.h"

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
        m_fp = static_cast<double>(static_cast<float>(m_fp) + static_cast<float>(value));
    }
    else if (m_type == ValueType::Double)
    {
        m_fp += value;
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
        m_fp = static_cast<double>(static_cast<float>(m_fp) - static_cast<float>(value));
    }
    else if (m_type == ValueType::Double)
    {
        m_fp -= value;
    }
    else
    {
        return VmError::RegisterMismatch;
    }

    return VmError::OK;
}

VmError Register::AddRegister(Register& rhs)
{


    return VmError::OK;
}

VmError Register::SubRegister(Register& rhs)
{

    return VmError::OK;
}