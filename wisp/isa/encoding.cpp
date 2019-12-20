#include "shared/types.h"
#include "integer_value.h"
#include "fp_value.h"
#include "register.h"
#include "context.h"
#include "vm/vm.h"
#include "isa.h"
#include "encoding.h"

using namespace wisp;
using namespace vmcore;

VmError encode::SetIntegerRegisterValueWithEncoding(Vm* vm, RegisterInt& reg, IntegerValueType encoding, uint64 base)
{
	switch (encoding)
	{
	case IntegerValueType::Int8:
		reg.Set<int8>(static_cast<int8>(base) + encode::ReadArgument<int8>(vm));
		break;
	case IntegerValueType::Int16:
		reg.Set<int16>(static_cast<int16>(base) + encode::ReadArgument<int16>(vm));
		break;
	case IntegerValueType::Int32:
		reg.Set<int32>(static_cast<int32>(base) + encode::ReadArgument<int32>(vm));
		break;
	case IntegerValueType::Int64:
		reg.Set<int64>(static_cast<int64>(base) + encode::ReadArgument<int64>(vm));
		break;
	case IntegerValueType::UInt8:
		reg.Set<uint8>(static_cast<uint8>(base) + encode::ReadArgument<uint8>(vm));
		break;
	case IntegerValueType::UInt16:
		reg.Set<uint16>(static_cast<uint16>(base) + encode::ReadArgument<uint16>(vm));
		break;
	case IntegerValueType::UInt32:
		reg.Set<uint32>(static_cast<uint32>(base) + encode::ReadArgument<uint32>(vm));
		break;
	case IntegerValueType::UInt64:
		reg.Set<uint64>(static_cast<uint64>(base) + encode::ReadArgument<uint64>(vm));
		break;
	default:
		return VmError::InvalidInstruction;
	}

	return VmError::OK;
}

VmError encode::GetIntegerConstantValueWithEncoding(Vm* vm, IntegerValue& value, IntegerValueType encoding)
{
	switch (encoding)
	{
	case IntegerValueType::Int8:
		value.Set<int8>(encode::ReadArgument<int8>(vm));
		break;
	case IntegerValueType::Int16:
		value.Set<int16>(encode::ReadArgument<int16>(vm));
		break;
	case IntegerValueType::Int32:
		value.Set<int32>(encode::ReadArgument<int32>(vm));
		break;
	case IntegerValueType::Int64:
		value.Set<int64>(encode::ReadArgument<int64>(vm));
		break;
	case IntegerValueType::UInt8:
		value.Set<uint8>(encode::ReadArgument<uint8>(vm));
		break;
	case IntegerValueType::UInt16:
		value.Set<uint16>(encode::ReadArgument<uint16>(vm));
		break;
	case IntegerValueType::UInt32:
		value.Set<uint32>(encode::ReadArgument<uint32>(vm));
		break;
	case IntegerValueType::UInt64:
		value.Set<uint64>(encode::ReadArgument<uint64>(vm));
		break;
	default:
		return VmError::InvalidInstruction;
	}

	return VmError::OK;
}

VmError encode::SetFPRegisterValueWithEncoding(Vm* vm, RegisterFP& reg, FPValueType encoding)
{
	switch (encoding)
	{
	case FPValueType::Float:
		reg.Set<float>(encode::ReadArgument<float>(vm));
		break;
	case FPValueType::Double:
		reg.Set<double>(encode::ReadArgument<double>(vm));
		break;
	default:
		return VmError::InvalidInstruction;
	}

	return VmError::OK;
}

VmError encode::GetRelativeAddressDestinationFromPc(vmcore::Vm* vm, WispContext* context, uint8 encoding, uint64 pc, uint64* addr)
{
	UNREFERENCED_PARAMETER(context);

	switch (static_cast<IntegerValueType>(encoding))
	{
	case IntegerValueType::Int8:
		*addr = static_cast<uint64>(static_cast<int64>(pc) + ReadArgument<int8>(vm));
		return VmError::OK;
	case IntegerValueType::Int16:
		*addr = static_cast<uint64>(static_cast<int64>(pc) + ReadArgument<int16>(vm));
		return VmError::OK;
	case IntegerValueType::Int32:
		*addr = static_cast<uint64>(static_cast<int64>(pc) + ReadArgument<int32>(vm));
		return VmError::OK;
	case IntegerValueType::Int64:
		*addr = static_cast<uint64>(static_cast<int64>(pc) + ReadArgument<int64>(vm));
		return VmError::OK;
	case IntegerValueType::UInt8:
		*addr = pc + ReadArgument<uint8>(vm);
		return VmError::OK;
	case IntegerValueType::UInt16:
		*addr = pc + ReadArgument<uint16>(vm);
		return VmError::OK;
	case IntegerValueType::UInt32:
		*addr = pc + ReadArgument<uint32>(vm);
		return VmError::OK;
	case IntegerValueType::UInt64:
		*addr = pc + ReadArgument<uint64>(vm);
		return VmError::OK;
	}

	return VmError::InvalidInstruction;
}