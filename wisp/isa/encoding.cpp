#include "shared/types.h"
#include "integer_value.h"
#include "fp_value.h"
#include "register.h"
#include "context.h"
#include "vm/vm.h"
#include "isa.h"
#include "encoding.h"
#include <iostream>

using namespace wisp;
using namespace vmcore;

static VmError ReadIntegerValueWithEncoding(Vm* vm, IntegerValueType type, Value& value)
{
	IntegerValue ival;

	switch (type)
	{
	case IntegerValueType::Int8:
		ival.Set<int8>(encode::ReadArgument<int8>(vm));
		break;
	case IntegerValueType::Int16:
		ival.Set<int16>(encode::ReadArgument<int16>(vm));
		break;
	case IntegerValueType::Int32:
		ival.Set<int32>(encode::ReadArgument<int32>(vm));
		break;
	case IntegerValueType::Int64:
		ival.Set<int64>(encode::ReadArgument<int64>(vm));
		break;
	case IntegerValueType::UInt8:
		ival.Set<uint8>(encode::ReadArgument<uint8>(vm));
		break;
	case IntegerValueType::UInt16:
		ival.Set<uint16>(encode::ReadArgument<uint16>(vm));
		break;
	case IntegerValueType::UInt32:
		ival.Set<uint32>(encode::ReadArgument<uint32>(vm));
		break;
	case IntegerValueType::UInt64:
		ival.Set<uint64>(encode::ReadArgument<uint64>(vm));
		break;
	default:
		return VmError::InvalidInstruction;
	}

	value.Set(ival);

	return VmError::OK;
}

static VmError ReadFloatingPointValueWithEncoding(vmcore::Vm* vm, FPValueType type, Value& value)
{
	FPValue fval;

	switch (type)
	{
	case FPValueType::Float:
		fval.Set<float>(encode::ReadArgument<float>(vm));
		break;
	case FPValueType::Double:
		fval.Set<double>(encode::ReadArgument<double>(vm));
		break;
	default:
		return VmError::InvalidInstruction;
	}

	value.Set(fval);

	return VmError::OK;
}

VmError encode::ReadValueWithEncoding(vmcore::Vm* vm, Value& value)
{
	uint8 group = ReadArgument<uint8>(vm);
	uint8 encoding = ReadArgument<uint8>(vm);

	switch (static_cast<ValueType>(group))
	{
	case ValueType::Integer:
		return ReadIntegerValueWithEncoding(vm, static_cast<IntegerValueType>(encoding), value);
	case ValueType::FP:
		return ReadFloatingPointValueWithEncoding(vm, static_cast<FPValueType>(encoding), value);
	default:
		assert(false);
		break;
	}

	return VmError::InvalidInstruction;
}

VmError encode::ReadValueWithEncodingToRegister(vmcore::Vm* vm, Register& reg)
{
	Value val;
	VmError err = ReadValueWithEncoding(vm, val);
	if (err != VmError::OK)
		return err;

	reg.SetValue(val);

	return VmError::OK;
}

VmError encode::ReadStringOffsetIntoRegister(vmcore::Vm* vm, Register& reg, IntegerValue& offset)
{
	StringValue sval;

	sval.SetString(std::visit([&vm](auto&& arg) -> std::string
	{
		std::string str;

		char c = 0;
		while (vm->GetMemory()->Read(arg, &c, 1))
		{
			if (c == 0)
				break;

			str.append(1, c);

			++arg;
		}

		return str;
	}, 
	offset.GetValue()));

	reg.SetValue(Value(sval));

	return VmError::OK;
}