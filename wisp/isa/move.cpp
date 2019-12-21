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

static VmError Move(WispISA* isa, Vm* vm, WispContext* context, uint64 instructionPc)
{
	UNREFERENCED_PARAMETER(isa);
	UNREFERENCED_PARAMETER(instructionPc);

	uint8 regIndex1 = encode::ReadArgument<uint8>(vm);
	uint8 regIndex2 = encode::ReadArgument<uint8>(vm);

	RegisterInt& reg1 = context->regGp[regIndex1];
	RegisterInt& reg2 = context->regGp[regIndex2];

	reg1.CopyValue(reg2);

	return VmError::OK;
}

static VmError MoveFP(WispISA* isa, Vm* vm, WispContext* context, uint64 instructionPc)
{
	UNREFERENCED_PARAMETER(isa);
	UNREFERENCED_PARAMETER(instructionPc);

	uint8 regIndex1 = encode::ReadArgument<uint8>(vm);
	uint8 regIndex2 = encode::ReadArgument<uint8>(vm);

	RegisterFP& reg1 = context->regFp[regIndex1];
	RegisterFP& reg2 = context->regFp[regIndex2];

	reg1.CopyValue(reg2);

	return VmError::OK;
}

static VmError MoveComplex(WispISA* isa, Vm* vm, WispContext* context, uint64 instructionPc)
{
	UNREFERENCED_PARAMETER(isa);
	UNREFERENCED_PARAMETER(vm);
	UNREFERENCED_PARAMETER(context);
	UNREFERENCED_PARAMETER(instructionPc);

	return VmError::OK;
}

static VmError MoveConstantInteger(WispISA* isa, Vm* vm, WispContext* context, uint64 instructionPc)
{
	UNREFERENCED_PARAMETER(isa);
	UNREFERENCED_PARAMETER(instructionPc);

	uint8 regIndex = encode::ReadArgument<uint8>(vm);
	RegisterInt& reg = context->regGp[regIndex];
	IntegerValueType encoding = static_cast<IntegerValueType>(encode::ReadArgument<uint8>(vm));
	return encode::SetIntegerRegisterValueWithEncoding(vm, reg, encoding);
}

static VmError MoveConstantFP(WispISA* isa, Vm* vm, WispContext* context, uint64 instructionPc)
{
	UNREFERENCED_PARAMETER(isa);
	UNREFERENCED_PARAMETER(instructionPc);

	uint8 regIndex = encode::ReadArgument<uint8>(vm);
	RegisterFP& reg = context->regFp[regIndex];
	FPValueType encoding = static_cast<FPValueType>(encode::ReadArgument<uint8>(vm));
	return encode::SetFPRegisterValueWithEncoding(vm, reg, encoding);
}

static VmError MoveRelative(WispISA* isa, Vm* vm, WispContext* context, uint64 instructionPc)
{
	UNREFERENCED_PARAMETER(isa);

	uint8 regIndex = encode::ReadArgument<uint8>(vm);
	RegisterInt& reg = context->regGp[regIndex];
	IntegerValueType encoding = static_cast<IntegerValueType>(encode::ReadArgument<uint8>(vm));
	return encode::SetIntegerRegisterValueWithEncoding(vm, reg, encoding, instructionPc);
}

static VmError ClearRegister(WispISA* isa, Vm* vm, WispContext* context, uint64 instructionPc)
{
	UNREFERENCED_PARAMETER(isa);
	UNREFERENCED_PARAMETER(instructionPc);

	uint8 reg = encode::ReadArgument<uint8>(vm);
	context->regGp[reg].DestroyValue();
	return VmError::OK;
}

vmcore::VmError MoveISAModule::Create(isa_fn* functionList)
{
	functionList[static_cast<uint32>(InstructionCodes::Move)] = Move;
	functionList[static_cast<uint32>(InstructionCodes::MoveFP)] = MoveFP;
	functionList[static_cast<uint32>(InstructionCodes::MoveComplex)] = MoveComplex;
	functionList[static_cast<uint32>(InstructionCodes::MoveConstantInteger)] = MoveConstantInteger;
	functionList[static_cast<uint32>(InstructionCodes::MoveConstantFP)] = MoveConstantFP;
	functionList[static_cast<uint32>(InstructionCodes::MoveRelative)] = MoveRelative;
	functionList[static_cast<uint32>(InstructionCodes::ClearRegister)] = ClearRegister;

	return vmcore::VmError::OK;
}