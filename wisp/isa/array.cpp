#include "shared/types.h"
#include "vm/vm.h"
#include "fp_value.h"
#include "integer_value.h"
#include "register.h"
#include "context.h"
#include "array.h"
#include "encoding.h"
#include "complex_value.h"
#include <iostream>

using namespace wisp;
using namespace vmcore;

static VmError ArrayCreate(WispISA* isa, WispISAModule* mod, Vm* vm, WispContext* context, uint64 instructionPc)
{
    UNREFERENCED_PARAMETER(isa);
    UNREFERENCED_PARAMETER(mod);
    UNREFERENCED_PARAMETER(instructionPc);

    Register& regDest = context->regGp[encode::ReadArgument<uint8>(vm)];
    uint8 arrayValueType = encode::ReadArgument<uint8>(vm);
    uint8 arraySpecificType = encode::ReadArgument<uint8>(vm);

    regDest.DestroyValue();

    switch (static_cast<ValueType>(arrayValueType))
    {
    case ValueType::Integer:
        regDest.Set(ArrayValue(static_cast<IntegerValueType>(arraySpecificType)));
        break;
    case ValueType::FP:
        regDest.Set(ArrayValue(static_cast<FPValueType>(arraySpecificType)));
        break;
    case ValueType::String:
        regDest.Set(ArrayValue(ComplexValueType::String));
        break;
    case ValueType::Table:
        regDest.Set(ArrayValue(ComplexValueType::Table));
        break;
    case ValueType::Array:
        regDest.Set(ArrayValue(ComplexValueType::Array));
        break;
    }

    return VmError::OK;
}

static VmError ArrayClear(WispISA* isa, WispISAModule* mod, Vm* vm, WispContext* context, uint64 instructionPc)
{
    UNREFERENCED_PARAMETER(isa);
    UNREFERENCED_PARAMETER(mod);
    UNREFERENCED_PARAMETER(instructionPc);

    Register& regDest = context->regGp[encode::ReadArgument<uint8>(vm)];

    regDest.Get<ArrayValue>().Clear();

    return VmError::OK;
}

static VmError ArrayIsEmpty(WispISA* isa, WispISAModule* mod, Vm* vm, WispContext* context, uint64 instructionPc)
{
    UNREFERENCED_PARAMETER(isa);
    UNREFERENCED_PARAMETER(mod);
    UNREFERENCED_PARAMETER(vm);
    UNREFERENCED_PARAMETER(context);
    UNREFERENCED_PARAMETER(instructionPc);
    return VmError::OK;
}

static VmError ArrayReserve(WispISA* isa, WispISAModule* mod, Vm* vm, WispContext* context, uint64 instructionPc)
{
    UNREFERENCED_PARAMETER(isa);
    UNREFERENCED_PARAMETER(mod);
    UNREFERENCED_PARAMETER(vm);
    UNREFERENCED_PARAMETER(context);
    UNREFERENCED_PARAMETER(instructionPc);
    return VmError::OK;
}

static VmError ArrayPush(WispISA* isa, WispISAModule* mod, Vm* vm, WispContext* context, uint64 instructionPc)
{
    UNREFERENCED_PARAMETER(isa);
    UNREFERENCED_PARAMETER(mod);
    UNREFERENCED_PARAMETER(vm);
    UNREFERENCED_PARAMETER(context);
    UNREFERENCED_PARAMETER(instructionPc);

    Register& regDest = context->regGp[encode::ReadArgument<uint8>(vm)];
    Register& regSrc = context->regGp[encode::ReadArgument<uint8>(vm)];

    regDest.Get<ArrayValue>().Push(regSrc.GetValue());

    return VmError::OK;
}

static VmError ArrayPop(WispISA* isa, WispISAModule* mod, Vm* vm, WispContext* context, uint64 instructionPc)
{
    UNREFERENCED_PARAMETER(isa);
    UNREFERENCED_PARAMETER(mod);
    UNREFERENCED_PARAMETER(vm);
    UNREFERENCED_PARAMETER(context);
    UNREFERENCED_PARAMETER(instructionPc);

    Register& regDest = context->regGp[encode::ReadArgument<uint8>(vm)];

    regDest.Get<ArrayValue>().Pop();

    return VmError::OK;
}

static VmError ArrayInsert(WispISA* isa, WispISAModule* mod, Vm* vm, WispContext* context, uint64 instructionPc)
{
    UNREFERENCED_PARAMETER(isa);
    UNREFERENCED_PARAMETER(mod);
    UNREFERENCED_PARAMETER(vm);
    UNREFERENCED_PARAMETER(context);
    UNREFERENCED_PARAMETER(instructionPc);
    return VmError::OK;
}

static VmError ArrayGetValueAtIndex(WispISA* isa, WispISAModule* mod, Vm* vm, WispContext* context, uint64 instructionPc)
{
    UNREFERENCED_PARAMETER(isa);
    UNREFERENCED_PARAMETER(mod);
    UNREFERENCED_PARAMETER(vm);
    UNREFERENCED_PARAMETER(context);
    UNREFERENCED_PARAMETER(instructionPc);
    return VmError::OK;
}

static VmError ArraySetValueAtIndex(WispISA* isa, WispISAModule* mod, Vm* vm, WispContext* context, uint64 instructionPc)
{
    UNREFERENCED_PARAMETER(isa);
    UNREFERENCED_PARAMETER(mod);
    UNREFERENCED_PARAMETER(vm);
    UNREFERENCED_PARAMETER(context);
    UNREFERENCED_PARAMETER(instructionPc);
    return VmError::OK;
}

static VmError ArrayEraseAtIndex(WispISA* isa, WispISAModule* mod, Vm* vm, WispContext* context, uint64 instructionPc)
{
    UNREFERENCED_PARAMETER(isa);
    UNREFERENCED_PARAMETER(mod);
    UNREFERENCED_PARAMETER(vm);
    UNREFERENCED_PARAMETER(context);
    UNREFERENCED_PARAMETER(instructionPc);
    return VmError::OK;
}

static VmError ArrayDestroy(WispISA* isa, WispISAModule* mod, Vm* vm, WispContext* context, uint64 instructionPc)
{
    UNREFERENCED_PARAMETER(isa);
    UNREFERENCED_PARAMETER(mod);
    UNREFERENCED_PARAMETER(vm);
    UNREFERENCED_PARAMETER(context);
    UNREFERENCED_PARAMETER(instructionPc);
    return VmError::OK;
}

VmError ArrayISAModule::Create(std::unordered_map<InstructionCodes, isa_fn>& functionList)
{
	functionList[InstructionCodes::ArrayCreate] = ArrayCreate;
    functionList[InstructionCodes::ArrayClear] = ArrayClear;
    functionList[InstructionCodes::ArrayIsEmpty] = ArrayIsEmpty;
    functionList[InstructionCodes::ArrayReserve] = ArrayReserve;
    functionList[InstructionCodes::ArrayPush] = ArrayPush;
    functionList[InstructionCodes::ArrayPop] = ArrayPop;
    functionList[InstructionCodes::ArrayInsert] = ArrayInsert;
    functionList[InstructionCodes::ArrayGetValueAtIndex] = ArrayGetValueAtIndex;
    functionList[InstructionCodes::ArraySetValueAtIndex] = ArraySetValueAtIndex;
    functionList[InstructionCodes::ArrayEraseAtIndex] = ArrayEraseAtIndex;
    functionList[InstructionCodes::ArrayDestroy] = ArrayDestroy;

	return vmcore::VmError::OK;
}

/*
        ArrayCreate,
        ArrayGet,
        ArrayClear,
        ArrayIsEmpty,
        ArrayReserve,
        ArrayPush,
        ArrayPop,
        ArrayInsert,
        ArrayGetValueAtIndex,
        ArraySetValueAtIndex,
        ArrayEraseAtIndex,
        ArrayDestroy,
*/