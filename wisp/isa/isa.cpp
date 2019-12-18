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
        instructionPc = static_cast<uint64>(static_cast<int64>(instructionPc) + isa->ReadArgument<int8>(vm));
        break;
    case ValueType::Int16:
        instructionPc = static_cast<uint64>(static_cast<int64>(instructionPc) + isa->ReadArgument<int16>(vm));
        break;
    case ValueType::Int32:
        instructionPc = static_cast<uint64>(static_cast<int64>(instructionPc) + isa->ReadArgument<int32>(vm));
        break;
    case ValueType::Int64:
        instructionPc = static_cast<uint64>(static_cast<int64>(instructionPc) + isa->ReadArgument<int64>(vm));
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

static VmError Jump(WispISA* isa, Vm* vm, WispContext* context, uint64 instructionPc)
{
    uint8 encoding = isa->ReadArgument<uint8>(vm);

    uint64 dest = 0;
    VmError err = isa->GetRelativeAddressDestinationFromPc(vm, context, encoding, instructionPc, &dest);
    if (err != VmError::OK)
        return err;

    context->regPc.GoTo(dest);

    return VmError::OK;
}

static VmError ConditionalJump(WispISA* isa, Vm* vm, WispContext* context, uint64 instructionPc)
{
    ConditionCode conditionCode = static_cast<ConditionCode>(isa->ReadArgument<uint8>(vm));

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

//

template<typename T>
void SetFlags(WispContext* context, const WispExecutionFlags& dirty, const T& dst, const T& src1, const T& src2)
{
    if (dirty.CarryFlag)
        context->eflags.CarryFlag = ((src1 ^ ((src1 ^ src2) & (src2 ^ dst))) >> sizeof(T) & 1);

    if (dirty.ParityFlag)
    {
        uint8 v = dst & 0xff;
        v ^= v >> 1;
        v ^= v >> 2;
        v = (v & 0x11111111U) * 0x11111111U;
        context->eflags.ParityFlag = (!((v >> 28) & 1));
    }

    if (dirty.ZeroFlag)
        context->eflags.ZeroFlag = (!dst);

    if (dirty.SignFlag)
        context->eflags.SignFlag = (dst >> sizeof(T) & 1);

    if (dirty.OverflowFlag)
        context->eflags.OverflowFlag = (((src1 ^ dst) & (src2 ^ dst)) >> sizeof(T) & 1);
}

template<typename T>
void SetFlags(WispContext* context, const T& dst, const T& src1, const T& src2)
{
    WispExecutionFlags ef;
    ef.CarryFlag = 1u;
    ef.ParityFlag = 1u;
    ef.ZeroFlag = 1u;
    ef.SignFlag = 1u;
    ef.OverflowFlag = 1u;
    SetFlags(context, ef, dst, src1, src2);
}

static VmError Compare(WispISA* isa, Vm* vm, WispContext* context, uint64 instructionPc)
{
    UNREFERENCED_PARAMETER(isa);
    UNREFERENCED_PARAMETER(vm);
    UNREFERENCED_PARAMETER(context);
    UNREFERENCED_PARAMETER(instructionPc);

    //

    return VmError::OK;
}

static VmError CompareConstant(WispISA* isa, Vm* vm, WispContext* context, uint64 instructionPc)
{
    UNREFERENCED_PARAMETER(isa);
    UNREFERENCED_PARAMETER(vm);
    UNREFERENCED_PARAMETER(context);
    UNREFERENCED_PARAMETER(instructionPc);

/*
temp <- SRC1 - SignExtend(SRC2);
ModifyStatusFlags; (* Modify status flags in the same manner as the SUB instruction*)
*/

    /*
    uint8 regIndex = isa->ReadArgument<uint8>(vm);
    Register& reg = context->regGeneral[regIndex];
    ValueType encoding = static_cast<ValueType>(isa->ReadArgument<uint8>(vm));

    switch (encoding)
    {
    case ValueType::Int8:


        instructionPc = static_cast<uint64>(static_cast<int64>(instructionPc) + isa->ReadArgument<int8>(vm));
        break;
    case ValueType::Int16:
        instructionPc = static_cast<uint64>(static_cast<int64>(instructionPc) + isa->ReadArgument<int16>(vm));
        break;
    case ValueType::Int32:
        instructionPc = static_cast<uint64>(static_cast<int64>(instructionPc) + isa->ReadArgument<int32>(vm));
        break;
    case ValueType::Int64:
        instructionPc = static_cast<uint64>(static_cast<int64>(instructionPc) + isa->ReadArgument<int64>(vm));
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
    }*/

    return VmError::OK;
}

static VmError Test(WispISA* isa, Vm* vm, WispContext* context, uint64 instructionPc)
{
    UNREFERENCED_PARAMETER(isa);
    UNREFERENCED_PARAMETER(vm);
    UNREFERENCED_PARAMETER(context);
    UNREFERENCED_PARAMETER(instructionPc);

/*
TEMP <- SRC1 AND SRC2;
SF <- MSB(TEMP);
IF TEMP = 0
THEN ZF <- 1;
ELSE ZF <- 0;
FI:
PF <- BitwiseXNOR(TEMP[0:7]);
CF <- 0;
OF <- 0;
(* AF is undefined *)
*/

    return VmError::OK;
}

static VmError TestConstant(WispISA* isa, Vm* vm, WispContext* context, uint64 instructionPc)
{
    UNREFERENCED_PARAMETER(isa);
    UNREFERENCED_PARAMETER(vm);
    UNREFERENCED_PARAMETER(context);
    UNREFERENCED_PARAMETER(instructionPc);

    return VmError::OK;
}

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

    // Comparison
    Compare,
    CompareConstant,
    Test,
    TestConstant,

    /*
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

VmError WispISA::GetRelativeAddressDestinationFromPc(vmcore::Vm* vm, WispContext* context, uint8 encoding, uint64 pc, uint64* addr)
{
    UNREFERENCED_PARAMETER(context);

    switch (static_cast<ValueType>(encoding))
    {
    case ValueType::Int8:
        *addr = static_cast<uint64>(static_cast<int64>(pc) + ReadArgument<int8>(vm));
        return VmError::OK;
    case ValueType::Int16:
        *addr = static_cast<uint64>(static_cast<int64>(pc) + ReadArgument<int16>(vm));
        return VmError::OK;
    case ValueType::Int32:
        *addr = static_cast<uint64>(static_cast<int64>(pc) + ReadArgument<int32>(vm));
        return VmError::OK;
    case ValueType::Int64:
        *addr = static_cast<uint64>(static_cast<int64>(pc) + ReadArgument<int64>(vm));
        return VmError::OK;
    case ValueType::UInt8:
        *addr = pc + ReadArgument<uint8>(vm);
        return VmError::OK;
    case ValueType::UInt16:
        *addr = pc + ReadArgument<uint16>(vm);
        return VmError::OK;
    case ValueType::UInt32:
        *addr = pc + ReadArgument<uint32>(vm);
        return VmError::OK;
    case ValueType::UInt64:
        *addr = pc + ReadArgument<uint64>(vm);
        return VmError::OK;
    }

    return VmError::InvalidInstruction;
}