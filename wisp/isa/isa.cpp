#include "shared/types.h"
#include "isa.h"
#include "vm/vm.h"
#include "../context.h"

using namespace wisp;
using namespace vmcore;

static VmError Move(WispISA* isa, Vm* vm, WispContext* context, uint64 instructionPc)
{
	UNREFERENCED_PARAMETER(instructionPc);

	uint8 regIndex1 = isa->ReadArgument<uint8>(vm);
	uint8 regIndex2 = isa->ReadArgument<uint8>(vm);

	Register& reg1 = context->regGeneral[regIndex1];
	Register& reg2 = context->regGeneral[regIndex2];

	reg1.CopyValue(reg2);

	return VmError::OK;
}

static VmError MoveConstantInteger(WispISA* isa, Vm* vm, WispContext* context, uint64 instructionPc)
{
	UNREFERENCED_PARAMETER(instructionPc);

	uint8 regIndex = isa->ReadArgument<uint8>(vm);
	Register& reg = context->regGeneral[regIndex];
	ValueType encoding = static_cast<ValueType>(isa->ReadArgument<uint8>(vm));

	switch (encoding)
	{
	case ValueType::Int8:
		reg.SetInt8(isa->ReadArgument<int8>(vm));
		break;
	case ValueType::Int16:
		reg.SetInt16(isa->ReadArgument<int16>(vm));
		break;
	case ValueType::Int32:
		reg.SetInt32(isa->ReadArgument<int32>(vm));
		break;
	case ValueType::Int64:
		reg.SetInt64(isa->ReadArgument<int64>(vm));
		break;
	case ValueType::UInt8:
		reg.SetUInt8(isa->ReadArgument<uint8>(vm));
		break;
	case ValueType::UInt16:
		reg.SetUInt16(isa->ReadArgument<uint16>(vm));
		break;
	case ValueType::UInt32:
		reg.SetUInt32(isa->ReadArgument<uint32>(vm));
		break;
	case ValueType::UInt64:
		reg.SetUInt64(isa->ReadArgument<uint64>(vm));
		break;
	default:
		return VmError::InvalidInstruction;
	}

	return VmError::OK;
}

static VmError MoveConstantFP(WispISA* isa, Vm* vm, WispContext* context, uint64 instructionPc)
{
	UNREFERENCED_PARAMETER(instructionPc);

	uint8 regIndex = isa->ReadArgument<uint8>(vm);
	Register& reg = context->regGeneral[regIndex];
	ValueType encoding = static_cast<ValueType>(isa->ReadArgument<uint8>(vm));

	switch (encoding)
	{
	case ValueType::Float:
		reg.SetFloat(isa->ReadArgument<float>(vm));
		break;
	case ValueType::Double:
		reg.SetDouble(isa->ReadArgument<double>(vm));
		break;
	default:
		return VmError::InvalidInstruction;
	}

	return VmError::OK;
}

static VmError MoveRelative(WispISA* isa, Vm* vm, WispContext* context, uint64 instructionPc)
{
	uint8 regIndex = isa->ReadArgument<uint8>(vm);
	Register& reg = context->regGeneral[regIndex];
	ValueType encoding = static_cast<ValueType>(isa->ReadArgument<uint8>(vm));

	switch (encoding)
	{
	case ValueType::Int8:
		instructionPc += isa->ReadArgument<int8>(vm);
		break;
	case ValueType::Int16:
		instructionPc += isa->ReadArgument<int16>(vm);
		break;
	case ValueType::Int32:
		instructionPc += isa->ReadArgument<int32>(vm);
		break;
	case ValueType::Int64:
		instructionPc += isa->ReadArgument<int64>(vm);
		break;
	case ValueType::UInt8:
		instructionPc += isa->ReadArgument<uint8>(vm);
		break;
	case ValueType::UInt16:
		instructionPc += isa->ReadArgument<uint16>(vm);
		break;
	case ValueType::UInt32:
		instructionPc += isa->ReadArgument<uint32>(vm);
		break;
	case ValueType::UInt64:
		instructionPc += isa->ReadArgument<uint64>(vm);
		break;
	default:
		return VmError::InvalidInstruction;
	}

	reg.SetUInt64(instructionPc);

	return VmError::OK;
}

static VmError ClearRegister(WispISA* isa, Vm* vm, WispContext* context, uint64 instructionPc)
{
	UNREFERENCED_PARAMETER(instructionPc);

	uint8 reg = isa->ReadArgument<uint8>(vm);

	context->regGeneral[reg].DestroyValue();

	return VmError::OK;
}

// 

static VmError Store(WispISA* isa, Vm* vm, WispContext* context, uint64 instructionPc)
{
	UNREFERENCED_PARAMETER(isa);
	UNREFERENCED_PARAMETER(vm);
	UNREFERENCED_PARAMETER(context);
	UNREFERENCED_PARAMETER(instructionPc);

	return VmError::OK;
}

static VmError Load(WispISA* isa, Vm* vm, WispContext* context, uint64 instructionPc)
{
	UNREFERENCED_PARAMETER(isa);
	UNREFERENCED_PARAMETER(vm);
	UNREFERENCED_PARAMETER(context);
	UNREFERENCED_PARAMETER(instructionPc);

	return VmError::OK;
}

// 

static VmError Jump(WispISA* isa, Vm* vm, WispContext* context, uint64 instructionPc)
{
	UNREFERENCED_PARAMETER(isa);
	UNREFERENCED_PARAMETER(vm);
	UNREFERENCED_PARAMETER(context);
	UNREFERENCED_PARAMETER(instructionPc);

	return VmError::OK;
}

static VmError ConditionalJump(WispISA* isa, Vm* vm, WispContext* context, uint64 instructionPc)
{
	UNREFERENCED_PARAMETER(isa);
	UNREFERENCED_PARAMETER(vm);
	UNREFERENCED_PARAMETER(context);
	UNREFERENCED_PARAMETER(instructionPc);

	return VmError::OK;
}

static VmError Call(WispISA* isa, Vm* vm, WispContext* context, uint64 instructionPc)
{
	UNREFERENCED_PARAMETER(isa);
	UNREFERENCED_PARAMETER(vm);
	UNREFERENCED_PARAMETER(context);
	UNREFERENCED_PARAMETER(instructionPc);

	return VmError::OK;
}

static VmError Return(WispISA* isa, Vm* vm, WispContext* context, uint64 instructionPc)
{
	UNREFERENCED_PARAMETER(isa);
	UNREFERENCED_PARAMETER(vm);
	UNREFERENCED_PARAMETER(context);
	UNREFERENCED_PARAMETER(instructionPc);

	return VmError::OK;
}

static VmError Halt(WispISA* isa, Vm* vm, WispContext* context, uint64 instructionPc)
{
	UNREFERENCED_PARAMETER(isa);
	UNREFERENCED_PARAMETER(vm);
	UNREFERENCED_PARAMETER(context);
	UNREFERENCED_PARAMETER(instructionPc);

	return VmError::HaltExecution;
}

//

/*
static VmError Compare(WispISA* isa, Vm* vm, WispContext* context, uint64 instructionPc)
{

}

static VmError Test(WispISA* isa, Vm* vm, WispContext* context, uint64 instructionPc)
{

}*/

std::function<VmError(WispISA*, Vm*, WispContext*, uint64)> g_definitionTable[] =
{
	// General Purpose
	Move,
	MoveConstantInteger,
	MoveRelative,
	MoveConstantFP,
	ClearRegister,

	// Memory
	Store,
	Load,
	
	// Control Flow
	Jump,
	ConditionalJump,
	Call,
	Return,
	Halt,

	/*
	// Comparison
	Compare,
	Test,

	// Tables

	// Arrays

	// Math (Register to Register only)
	Add,
	Sub,
	Mul,
	Mod,
	Pow,
	Div,
	IDiv,
	BAnd,
	BOr,
	BXor,
	Shl,
	Shr,
	Unm,
	BNot,
	Not
	*/
};

VmError WispISA::ExecuteInstruction(Vm* vm)
{
	WispContext* context = dynamic_cast<WispContext*>(vm->GetContext());
	if (context == nullptr)
	{
		return VmError::InvalidContext;
	}

	uint64 startingPc = context->regPc.Get();

	const uint16 id = ReadArgument<uint16>(vm);

	return g_definitionTable[id](this, vm, context, startingPc);
}