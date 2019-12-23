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

static VmError Store(WispISA* isa, WispISAModule* mod, Vm* vm, WispContext* context, uint64 instructionPc)
{
	UNREFERENCED_PARAMETER(isa);
	UNREFERENCED_PARAMETER(mod);
	UNREFERENCED_PARAMETER(vm);
	UNREFERENCED_PARAMETER(context);
	UNREFERENCED_PARAMETER(instructionPc);

	return VmError::OK;
}

static VmError Load(WispISA* isa, WispISAModule* mod, Vm* vm, WispContext* context, uint64 instructionPc)
{
	UNREFERENCED_PARAMETER(isa);
	UNREFERENCED_PARAMETER(mod);
	UNREFERENCED_PARAMETER(vm);
	UNREFERENCED_PARAMETER(context);
	UNREFERENCED_PARAMETER(instructionPc);

	return VmError::OK;
}

vmcore::VmError LoadStoreISAModule::Create(std::unordered_map<InstructionCodes, isa_fn>& functionList)
{
	functionList[InstructionCodes::Store] = Store;
	functionList[InstructionCodes::Load] = Load;

	return vmcore::VmError::OK;
}