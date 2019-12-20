#include "shared/types.h"
#include "isa.h"
#include "vm/vm.h"
#include "../context.h"

using namespace wisp;
using namespace vmcore;

static VmError SetIntegerRegisterValueWithEncoding(WispISA* isa, Vm* vm, RegisterInt& reg, IntegerValueType encoding, uint64 base = 0u)
{
    switch (encoding)
    {
    case IntegerValueType::Int8:
        reg.Set<int8>(static_cast<int8>(base) + isa->ReadArgument<int8>(vm));
        break;
    case IntegerValueType::Int16:
        reg.Set<int16>(static_cast<int16>(base) + isa->ReadArgument<int16>(vm));
        break;
    case IntegerValueType::Int32:
        reg.Set<int32>(static_cast<int32>(base) + isa->ReadArgument<int32>(vm));
        break;
    case IntegerValueType::Int64:
        reg.Set<int64>(static_cast<int64>(base) + isa->ReadArgument<int64>(vm));
        break;
    case IntegerValueType::UInt8:
        reg.Set<uint8>(static_cast<uint8>(base) + isa->ReadArgument<uint8>(vm));
        break;
    case IntegerValueType::UInt16:
        reg.Set<uint16>(static_cast<uint16>(base) + isa->ReadArgument<uint16>(vm));
        break;
    case IntegerValueType::UInt32:
        reg.Set<uint32>(static_cast<uint32>(base) + isa->ReadArgument<uint32>(vm));
        break;
    case IntegerValueType::UInt64:
        reg.Set<uint64>(static_cast<uint64>(base) + isa->ReadArgument<uint64>(vm));
        break;
    default:
        return VmError::InvalidInstruction;
    }

    return VmError::OK;
}

static VmError GetIntegerConstantValueWithEncoding(WispISA* isa, Vm* vm, IntegerValue& value, IntegerValueType encoding)
{
    switch (encoding)
    {
    case IntegerValueType::Int8:
        value.Set<int8>(isa->ReadArgument<int8>(vm));
        break;
    case IntegerValueType::Int16:
        value.Set<int16>(isa->ReadArgument<int16>(vm));
        break;
    case IntegerValueType::Int32:
        value.Set<int32>(isa->ReadArgument<int32>(vm));
        break;
    case IntegerValueType::Int64:
        value.Set<int64>(isa->ReadArgument<int64>(vm));
        break;
    case IntegerValueType::UInt8:
        value.Set<uint8>(isa->ReadArgument<uint8>(vm));
        break;
    case IntegerValueType::UInt16:
        value.Set<uint16>(isa->ReadArgument<uint16>(vm));
        break;
    case IntegerValueType::UInt32:
        value.Set<uint32>(isa->ReadArgument<uint32>(vm));
        break;
    case IntegerValueType::UInt64:
        value.Set<uint64>(isa->ReadArgument<uint64>(vm));
        break;
    default:
        return VmError::InvalidInstruction;
    }

    return VmError::OK;
}

static VmError SetFPRegisterValueWithEncoding(WispISA* isa, Vm* vm, RegisterFP& reg, FPValueType encoding)
{
    switch (encoding)
    {
    case FPValueType::Float:
        reg.Set<float>(isa->ReadArgument<float>(vm));
        break;
    case FPValueType::Double:
        reg.Set<double>(isa->ReadArgument<double>(vm));
        break;
    default:
        return VmError::InvalidInstruction;
    }

    return VmError::OK;
}

static VmError Move(WispISA* isa, Vm* vm, WispContext* context, uint64 instructionPc)
{
    UNREFERENCED_PARAMETER(instructionPc);

    uint8 regIndex1 = isa->ReadArgument<uint8>(vm);
    uint8 regIndex2 = isa->ReadArgument<uint8>(vm);

    RegisterInt& reg1 = context->regGp[regIndex1];
    RegisterInt& reg2 = context->regGp[regIndex2];

    reg1.CopyValue(reg2);

    return VmError::OK;
}

static VmError MoveConstantInteger(WispISA* isa, Vm* vm, WispContext* context, uint64 instructionPc)
{
    UNREFERENCED_PARAMETER(instructionPc);

    uint8 regIndex = isa->ReadArgument<uint8>(vm);
    RegisterInt& reg = context->regGp[regIndex];
    IntegerValueType encoding = static_cast<IntegerValueType>(isa->ReadArgument<uint8>(vm));
    return SetIntegerRegisterValueWithEncoding(isa, vm, reg, encoding);
}

static VmError MoveConstantFP(WispISA* isa, Vm* vm, WispContext* context, uint64 instructionPc)
{
    UNREFERENCED_PARAMETER(instructionPc);

    uint8 regIndex = isa->ReadArgument<uint8>(vm);
    RegisterFP& reg = context->regFp[regIndex];
    FPValueType encoding = static_cast<FPValueType>(isa->ReadArgument<uint8>(vm));
    return SetFPRegisterValueWithEncoding(isa, vm, reg, encoding);
}

static VmError MoveRelative(WispISA* isa, Vm* vm, WispContext* context, uint64 instructionPc)
{
    uint8 regIndex = isa->ReadArgument<uint8>(vm);
    RegisterInt& reg = context->regGp[regIndex];
    IntegerValueType encoding = static_cast<IntegerValueType>(isa->ReadArgument<uint8>(vm));
    return SetIntegerRegisterValueWithEncoding(isa, vm, reg, encoding, instructionPc);
}

static VmError ClearRegister(WispISA* isa, Vm* vm, WispContext* context, uint64 instructionPc)
{
    UNREFERENCED_PARAMETER(instructionPc);
    uint8 reg = isa->ReadArgument<uint8>(vm);
    context->regGp[reg].DestroyValue();
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

VmError SetFlagsForIntegerOperation(WispContext* context, const WispExecutionFlags& dirty, IntegerValue& dst, IntegerValue& src1, IntegerValue& src2)
{
    static uint32 parity_table[8] =
    {
        0x96696996,
        0x69969669,
        0x69969669,
        0x96696996,
        0x69969669,
        0x96696996,
        0x96696996,
        0x69969669,
    };

    // We'll handle the SF/ZF/PF first.
    if (dirty.SignFlag)
    {
        std::visit([context](auto&& destination)
        {
            typedef decltype(destination) dtype;
            dtype signMaskShift = static_cast<dtype>((sizeof(dtype) * 8) - 1);
            dtype signMask = (static_cast<dtype>(1) << signMaskShift);
            context->eflags.SignFlag = ((destination & signMask) == signMask);
        }, dst.GetValue());
    }

    if (dirty.ZeroFlag)
    {
        std::visit([context](auto&& destination)
        {
            typedef decltype(destination) dtype;
            dtype neg = static_cast<dtype>(~0);
            context->eflags.ZeroFlag = ((destination & neg) == 0);
        }, dst.GetValue());
    }

    if (dirty.ParityFlag)
    {
        std::visit([context](auto&& destination)
        {
            auto piece = destination & 0xff;

            context->eflags.ParityFlag = (((parity_table[piece / 32] >> (piece % 32)) & 1) == 0);
        }, dst.GetValue());
    }

    IntegerValue borrowChain = (dst & (~src1 | src2)) | (~src1 & src2);

    // CF/OF happens here
    if (dirty.CarryFlag)
    {
        std::visit([context](auto&& destination)
        {
            typedef decltype(destination) dtype;
            dtype signMaskShift = static_cast<dtype>((sizeof(dtype) * 8) - 1);
            dtype signMask = (static_cast<dtype>(1) << signMaskShift);
            context->eflags.CarryFlag = ((destination & signMask) == signMask);
        }, borrowChain.GetValue());
    }

    if (dirty.OverflowFlag)
    {
        std::visit([context](auto&& destination)
        {
            typedef decltype(destination) dtype;
            dtype overflowMaskShift = static_cast<dtype>((sizeof(dtype) * 8) - 2);
            dtype xorMask = (destination >> overflowMaskShift);
            context->eflags.CarryFlag = (((xorMask) ^ ((xorMask) >> 1)) & 1);
        }, borrowChain.GetValue());
    }

    return VmError::OK;
}

VmError SetFlagsForIntegerOperation(WispContext* context, IntegerValue& dst, IntegerValue& src1, IntegerValue& src2)
{
    WispExecutionFlags ef;
    ef.CarryFlag = 1u;
    ef.ParityFlag = 1u;
    ef.ZeroFlag = 1u;
    ef.SignFlag = 1u;
    ef.OverflowFlag = 1u;
    return SetFlagsForIntegerOperation(context, ef, dst, src1, src2);
}

static VmError Compare(WispISA* isa, Vm* vm, WispContext* context, uint64 instructionPc)
{
    UNREFERENCED_PARAMETER(instructionPc);

    RegisterInt r1 = context->regGp[isa->ReadArgument<uint8>(vm)];
    RegisterInt r2 = context->regGp[isa->ReadArgument<uint8>(vm)];

    IntegerValue result = r1.GetValue() - r2.GetValue();

    // the result could have "any" type in the variant type...
    SetFlagsForIntegerOperation(context, result, r1.GetValue(), r2.GetValue());

    return VmError::OK;
}

static VmError CompareConstant(WispISA* isa, Vm* vm, WispContext* context, uint64 instructionPc)
{
    UNREFERENCED_PARAMETER(isa);
    UNREFERENCED_PARAMETER(vm);
    UNREFERENCED_PARAMETER(context);
    UNREFERENCED_PARAMETER(instructionPc);

    RegisterInt r1 = context->regGp[isa->ReadArgument<uint8>(vm)];
    IntegerValueType encoding = static_cast<IntegerValueType>(isa->ReadArgument<uint8>(vm));

    IntegerValue c1;
    VmError err = GetIntegerConstantValueWithEncoding(isa, vm, c1, encoding);
    if (err != VmError::OK)
        return err;

    IntegerValue result = r1.GetValue() - c1.GetValue();
    
    SetFlagsForIntegerOperation(context, result, r1.GetValue(), c1);

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

    const uint8 id = ReadArgument<uint8>(vm);

    return g_definitionTable[id](this, vm, context, startingPc);
}

VmError WispISA::GetRelativeAddressDestinationFromPc(vmcore::Vm* vm, WispContext* context, uint8 encoding, uint64 pc, uint64* addr)
{
    UNREFERENCED_PARAMETER(context);

    switch (static_cast<IntegerValueType>(encoding))
    {
    case IntegerValueType::Int8:
        *addr = static_cast<uint64>(static_cast<int64>(pc) + ReadArgument<int8>(vm));
        return VmError::OK;
    case IntegerValueType::Int16:
        *addr = static_cast<uint64>(static_cast<int64>(pc) + ReadArgument<int16>(vm));
        return VmError::OK;
    case IntegerValueType::Int32:
        *addr = static_cast<uint64>(static_cast<int64>(pc) + ReadArgument<int32>(vm));
        return VmError::OK;
    case IntegerValueType::Int64:
        *addr = static_cast<uint64>(static_cast<int64>(pc) + ReadArgument<int64>(vm));
        return VmError::OK;
    case IntegerValueType::UInt8:
        *addr = pc + ReadArgument<uint8>(vm);
        return VmError::OK;
    case IntegerValueType::UInt16:
        *addr = pc + ReadArgument<uint16>(vm);
        return VmError::OK;
    case IntegerValueType::UInt32:
        *addr = pc + ReadArgument<uint32>(vm);
        return VmError::OK;
    case IntegerValueType::UInt64:
        *addr = pc + ReadArgument<uint64>(vm);
        return VmError::OK;
    }

    return VmError::InvalidInstruction;
}