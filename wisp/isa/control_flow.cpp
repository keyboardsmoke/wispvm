#include "shared/types.h"
#include "vm/vm.h"
#include "fp_value.h"
#include "integer_value.h"
#include "register.h"
#include "context.h"
#include "control_flow.h"
#include "encoding.h"

using namespace wisp;
using namespace vmcore;

static VmError Jump(WispISA* isa, Vm* vm, WispContext* context, uint64 instructionPc)
{
	UNREFERENCED_PARAMETER(isa);

	Value delta;
	VmError err = encode::ReadValueWithEncoding(vm, delta);
	if (err != VmError::OK)
		return err;

	assert(delta.IsIntegerValue());

	IntegerValue deltaInteger = delta.Get<IntegerValue>();

	uint64 pc = instructionPc;

	std::visit([&pc](auto&& arg)
	{
		pc = static_cast<uint64>(
			(std::is_signed_v<decltype(arg)> ? static_cast<int64>(pc) : pc) + arg
		);
	}, 
	deltaInteger.GetValue());

	context->regPc.GoTo(pc);

	return VmError::OK;
}

static VmError ConditionalJump(WispISA* isa, Vm* vm, WispContext* context, uint64 instructionPc)
{
	ConditionCode conditionCode = static_cast<ConditionCode>(encode::ReadArgument<uint8>(vm));

	uint8 shouldSwitchPc = 0u;

	switch (conditionCode)
	{
	case ConditionCode::Overflow: if (context->eflags.OverflowFlag == 1) { shouldSwitchPc = 1u; } break;
	case ConditionCode::NotOverflow: if (context->eflags.OverflowFlag == 0) { shouldSwitchPc = 1u; } break;
	case ConditionCode::LessThan: if (context->eflags.CarryFlag == 1) { shouldSwitchPc = 1u; } break;
	case ConditionCode::GreaterThan: if (context->eflags.CarryFlag == 0 && context->eflags.ZeroFlag == 0) { shouldSwitchPc = 1u; } break;
	case ConditionCode::GreaterThanOrEqual: if (context->eflags.CarryFlag == 0) { shouldSwitchPc = 1u; } break;
	case ConditionCode::LessThanOrEqual: if (context->eflags.CarryFlag == 1 || context->eflags.ZeroFlag == 1) { shouldSwitchPc = 1u; } break;
	case ConditionCode::Zero: if (context->eflags.ZeroFlag == 1) { shouldSwitchPc = 1u; } break;
	case ConditionCode::NotZero: if (context->eflags.ZeroFlag == 0) { shouldSwitchPc = 1u; } break;
	case ConditionCode::Signed: if (context->eflags.SignFlag == 1) { shouldSwitchPc = 1u; } break;
	case ConditionCode::NotSigned: if (context->eflags.SignFlag == 0) { shouldSwitchPc = 1u; } break;
	case ConditionCode::Parity: if (context->eflags.ParityFlag == 1) { shouldSwitchPc = 1u; } break;
	case ConditionCode::NotParity: if (context->eflags.ParityFlag == 0) { shouldSwitchPc = 1u; } break;
	default:
		return VmError::InvalidInstruction;
	}

	if (shouldSwitchPc == 1u)
	{
		return Jump(isa, vm, context, instructionPc);
	}

	// If condition is not met, fall through...
	return VmError::OK;
}

static VmError Call(WispISA* isa, Vm* vm, WispContext* context, uint64 instructionPc)
{
	UNREFERENCED_PARAMETER(isa);
	UNREFERENCED_PARAMETER(vm);
	UNREFERENCED_PARAMETER(context);
	UNREFERENCED_PARAMETER(instructionPc);

	// TODO: If we want to do calls, we need a stack. So.... maybe get on that.

	return VmError::OK;
}

static VmError Return(WispISA* isa, Vm* vm, WispContext* context, uint64 instructionPc)
{
	UNREFERENCED_PARAMETER(isa);
	UNREFERENCED_PARAMETER(vm);
	UNREFERENCED_PARAMETER(context);
	UNREFERENCED_PARAMETER(instructionPc);

	// TODO: If we want to do calls, we need a stack. So.... maybe get on that.

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

vmcore::VmError ControlFlowISAModule::Create(isa_fn* functionList)
{
	functionList[static_cast<uint32>(InstructionCodes::Jump)] = Jump;
	functionList[static_cast<uint32>(InstructionCodes::ConditionalJump)] = ConditionalJump;
	functionList[static_cast<uint32>(InstructionCodes::Call)] = Call;
	functionList[static_cast<uint32>(InstructionCodes::Return)] = Return;
	functionList[static_cast<uint32>(InstructionCodes::Halt)] = Halt;

	return vmcore::VmError::OK;
}