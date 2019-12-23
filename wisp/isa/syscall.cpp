#include "shared/types.h"
#include "vm/vm.h"
#include "fp_value.h"
#include "integer_value.h"
#include "register.h"
#include "context.h"
#include "syscall.h"
#include "encoding.h"
#include "string_formatter.h"
#include <iostream>

using namespace wisp;
using namespace vmcore;

static VmError SyscallExit(WispISA* isa, WispISAModule* mod, Vm* vm, WispContext* context, uint64 instructionPc)
{
	UNREFERENCED_PARAMETER(isa);
	UNREFERENCED_PARAMETER(mod);
	UNREFERENCED_PARAMETER(vm);
	UNREFERENCED_PARAMETER(context);
	UNREFERENCED_PARAMETER(instructionPc);

	return VmError::HaltExecution;
}

static VmError SyscallPrint(WispISA* isa, WispISAModule* mod, Vm* vm, WispContext* context, uint64 instructionPc)
{
	UNREFERENCED_PARAMETER(isa);
	UNREFERENCED_PARAMETER(mod);
	UNREFERENCED_PARAMETER(vm);
	UNREFERENCED_PARAMETER(instructionPc);

	Register& fmt = context->regGp[0];

	assert(fmt.GetValue().IsStringValue());

	return VmError::OK;
}

static VmError SystemCall(WispISA* isa, WispISAModule* mod, Vm* vm, WispContext* context, uint64 instructionPc)
{
	UNREFERENCED_PARAMETER(isa);
	UNREFERENCED_PARAMETER(context);
	UNREFERENCED_PARAMETER(instructionPc);

	// We know it's us doing it, since we registered this function. No need for dynamic_cast cost.
	SyscallISAModule* isaModule = reinterpret_cast<SyscallISAModule*>(mod);

	uint8 syscallIndex = encode::ReadArgument<uint8>(vm);

	if (syscallIndex >= static_cast<uint8>(SystemCallIndices::Count))
	{
		return VmError::InvalidInstruction;
	}

	return isaModule->GetSystemCallProcedures()[static_cast<SystemCallIndices>(syscallIndex)](isa, mod, vm, context, instructionPc);
}

vmcore::VmError SyscallISAModule::Create(std::unordered_map<InstructionCodes, isa_fn>& functionList)
{
	functionList[InstructionCodes::SystemCall] = SystemCall;

	m_syscall[SystemCallIndices::Exit] = SyscallExit;
	m_syscall[SystemCallIndices::Print] = SyscallPrint;

	return vmcore::VmError::OK;
}