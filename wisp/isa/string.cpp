#include "shared/types.h"
#include "vm/vm.h"
#include "fp_value.h"
#include "integer_value.h"
#include "register.h"
#include "context.h"
#include "string.h"
#include "encoding.h"
#include "complex_value.h"

using namespace wisp;
using namespace vmcore;

static VmError StringCreate(WispISA* isa, WispISAModule* mod, Vm* vm, WispContext* context, uint64 instructionPc)
{
    UNREFERENCED_PARAMETER(isa);
    UNREFERENCED_PARAMETER(mod);
    UNREFERENCED_PARAMETER(vm);
    UNREFERENCED_PARAMETER(context);
    UNREFERENCED_PARAMETER(instructionPc);

    Register& regDest = context->regGp[encode::ReadArgument<uint8>(vm)];
    Register& regSource = context->regGp[encode::ReadArgument<uint8>(vm)];

    assert(regSource.GetValue().IsIntegerValue());

    regDest.DestroyValue();

    IntegerValue stringOffsetValue = regSource.GetValue().Get<IntegerValue>();

    return encode::ReadStringOffsetIntoRegister(vm, regDest, stringOffsetValue);
}

static VmError StringSet(WispISA* isa, WispISAModule* mod, Vm* vm, WispContext* context, uint64 instructionPc)
{
    UNREFERENCED_PARAMETER(isa);
    UNREFERENCED_PARAMETER(mod);
    UNREFERENCED_PARAMETER(vm);
    UNREFERENCED_PARAMETER(context);
    UNREFERENCED_PARAMETER(instructionPc);

    return VmError::OK;
}

static VmError StringGet(WispISA* isa, WispISAModule* mod, Vm* vm, WispContext* context, uint64 instructionPc)
{
    UNREFERENCED_PARAMETER(isa);
    UNREFERENCED_PARAMETER(mod);
    UNREFERENCED_PARAMETER(vm);
    UNREFERENCED_PARAMETER(context);
    UNREFERENCED_PARAMETER(instructionPc);

    return VmError::OK;
}

static VmError StringClear(WispISA* isa, WispISAModule* mod, Vm* vm, WispContext* context, uint64 instructionPc)
{
    UNREFERENCED_PARAMETER(isa);
    UNREFERENCED_PARAMETER(mod);
    UNREFERENCED_PARAMETER(vm);
    UNREFERENCED_PARAMETER(context);
    UNREFERENCED_PARAMETER(instructionPc);

    return VmError::OK;
}

static VmError StringIsEmpty(WispISA* isa, WispISAModule* mod, Vm* vm, WispContext* context, uint64 instructionPc)
{
    UNREFERENCED_PARAMETER(isa);
    UNREFERENCED_PARAMETER(mod);
    UNREFERENCED_PARAMETER(vm);
    UNREFERENCED_PARAMETER(context);
    UNREFERENCED_PARAMETER(instructionPc);

    return VmError::OK;
}

static VmError StringAppend(WispISA* isa, WispISAModule* mod, Vm* vm, WispContext* context, uint64 instructionPc)
{
    UNREFERENCED_PARAMETER(isa);
    UNREFERENCED_PARAMETER(mod);
    UNREFERENCED_PARAMETER(vm);
    UNREFERENCED_PARAMETER(context);
    UNREFERENCED_PARAMETER(instructionPc);

    return VmError::OK;
}

vmcore::VmError StringISAModule::Create(std::unordered_map<InstructionCodes, isa_fn>& functionList)
{
    functionList[InstructionCodes::StringCreate] = StringCreate;
    functionList[InstructionCodes::StringSet] = StringSet;
    functionList[InstructionCodes::StringGet] = StringGet;
    functionList[InstructionCodes::StringClear] = StringClear;
    functionList[InstructionCodes::StringIsEmpty] = StringIsEmpty;
    functionList[InstructionCodes::StringAppend] = StringAppend;

    return vmcore::VmError::OK;
}