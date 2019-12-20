#include "shared/types.h"
#include "vm/vm.h"
#include "fp_value.h"
#include "integer_value.h"
#include "register.h"
#include "context.h"
#include "load_store.h"
#include "encoding.h"

using namespace wisp;
using namespace vmcore;

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

vmcore::VmError LoadStoreISAModule::Create(isa_fn* functionList)
{
	functionList[static_cast<uint32>(InstructionCodes::Store)] = Store;
	functionList[static_cast<uint32>(InstructionCodes::Load)] = Load;

	return vmcore::VmError::OK;
}