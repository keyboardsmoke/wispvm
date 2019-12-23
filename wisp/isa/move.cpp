#include "shared/types.h"
#include "vm/vm.h"
#include "fp_value.h"
#include "integer_value.h"
#include "register.h"
#include "context.h"
#include "move.h"
#include "encoding.h"

using namespace wisp;
using namespace vmcore;

static VmError Move(WispISA* isa, WispISAModule* mod, Vm* vm, WispContext* context, uint64 instructionPc)
{
	UNREFERENCED_PARAMETER(isa);
	UNREFERENCED_PARAMETER(mod);
	UNREFERENCED_PARAMETER(instructionPc);

	uint8 regIndex1 = encode::ReadArgument<uint8>(vm);
	uint8 regIndex2 = encode::ReadArgument<uint8>(vm);

	Register& reg1 = context->regGp[regIndex1];
	Register& reg2 = context->regGp[regIndex2];

	reg1.CopyValue(reg2);

	return VmError::OK;
}

static VmError MoveConstant(WispISA* isa, WispISAModule* mod, Vm* vm, WispContext* context, uint64 instructionPc)
{
	UNREFERENCED_PARAMETER(isa);
	UNREFERENCED_PARAMETER(mod);
	UNREFERENCED_PARAMETER(instructionPc);

	uint8 regIndex = encode::ReadArgument<uint8>(vm);
	Register& reg = context->regGp[regIndex];
	return encode::ReadValueWithEncodingToRegister(vm, reg);
}

static VmError MoveRelative(WispISA* isa, WispISAModule* mod, Vm* vm, WispContext* context, uint64 instructionPc)
{
	UNREFERENCED_PARAMETER(isa);
	UNREFERENCED_PARAMETER(mod);

	uint8 regIndex = encode::ReadArgument<uint8>(vm);
	Register& reg = context->regGp[regIndex];

	Value value;
	VmError err = encode::ReadValueWithEncoding(vm, value);
	if (err != VmError::OK)
		return err;

	assert(value.IsIntegerValue());

	IntegerValue ival = value.Get<IntegerValue>();

	uint64 target = std::visit([instructionPc](auto&& arg) -> uint64
	{
		return static_cast<uint64>(
			(std::is_signed_v<decltype(arg)> ? static_cast<int64>(instructionPc) : instructionPc) + arg
		);
	}, 
	ival.GetValue());

	IntegerValue destValue;
	destValue.Set(target);
	reg.Set(destValue);

	return VmError::OK;
}

static VmError ClearRegister(WispISA* isa, WispISAModule* mod, Vm* vm, WispContext* context, uint64 instructionPc)
{
	UNREFERENCED_PARAMETER(isa);
	UNREFERENCED_PARAMETER(mod);
	UNREFERENCED_PARAMETER(instructionPc);

	uint8 reg = encode::ReadArgument<uint8>(vm);
	context->regGp[reg].DestroyValue();
	return VmError::OK;
}

vmcore::VmError MoveISAModule::Create(std::unordered_map<InstructionCodes, isa_fn>& functionList)
{
	functionList[InstructionCodes::Move] = Move;
	functionList[InstructionCodes::MoveConstant] = MoveConstant;
	functionList[InstructionCodes::MoveRelative] = MoveRelative;
	functionList[InstructionCodes::ClearRegister] = ClearRegister;

	return vmcore::VmError::OK;
}