#pragma once

#include "vm/context.h"
#include "register.h"

namespace wisp
{
    // Based on LLVM's predicate system
    enum class FloatPredicate
    {
        False,
        OrderedAndEqual,
        OrderedAndGreaterThan,
        OrderedAndGreaterThanOrEqual,
        OrderedAndLessThan,
        OrderedAndLessThanOrEqual,
        OrderedAndOperandsAreNotEqual,
        Ordered,
        Unordered,
        UnorderedOrEqual,
        UnorderedOrGreaterThan,
        UnorderedOrGreaterThanOrEqual,
        UnorderedOrLessThan,
        UnorderedOrLessThanOrEqual,
        UnorderedOrNotEqual,
        True
    };

    enum class IntegerPredicate
    {
        Equal,
        NotEqual,
        
        UnsignedGreaterThan,
        UnsignedGreaterOrEqual,
        UnsignedLessThan,
        UnsignedLessOrEqual,

        SignedGreaterThan,
        SignedGreaterOrEqual,
        SignedLessThan,
        SignedLessOrEqual
    };

    struct WispExecutionFlags
    {
        WispExecutionFlags() :
            CarryFlag(0), ParityFlag(0), ZeroFlag(0), 
            SignFlag(0), OverflowFlag(0), Reserved(0)
        {
        }

        void Reset()
        {
            CarryFlag = 0;
            ParityFlag = 0;
            ZeroFlag = 0;
            SignFlag = 0;
            OverflowFlag = 0;
            Reserved = 0;
        }

        uint8 CarryFlag : 1;
        uint8 ParityFlag : 1;
        uint8 ZeroFlag : 1;
        uint8 SignFlag : 1;
        uint8 OverflowFlag : 1;
        uint8 Reserved : 3;
    };
    static_assert(sizeof(WispExecutionFlags) == sizeof(uint8), "Invalid size for WispExecutionFlags");

    enum class GeneralPurposeRegisters
    {
        R0 = 0,
        R1,
        R2,
        R3,
        R4,
        R5,
        R6,
        R7,
        R8,
        R9,
        R10,
        R11,
        R12,
        R13,
        R14,
        R15,
        R16,
        R17,
        R18,
        R19,
        R20,
        R21,
        R22,
        R23,
        R24,
        R25,
        R26,
        R27,
        R28,
        R29,
        R30,
        R31,
        Count
    };

    enum class FloatingPointRegisters
    {
        FP0,
        FP1,
        FP2,
        FP3,
        FP4,
        FP5,
        FP6,
        FP7,
        FP8,
        FP9,
        FP10,
        FP11,
        FP12,
        FP13,
        FP14,
        FP15,
        Count
    };

    enum class ArgumentRegisters
    {
        ARG0,
        ARG1,
        ARG2,
        ARG3,
        ARG4,
        ARG5,
        ARG6,
        ARG7,
        ARG8,
        ARG9,
        ARG10,
        ARG11,
        ARG12,
        ARG13,
        ARG14,
        ARG15,
        Count
    };

    class WispContext : public vmcore::Context
    {
    public:
        // uint8 GetRegisterId(RegisterInt& reg);
        // uint8 GetRegisterId(RegisterFP& reg);

        // General purpose registers
        RegisterInt regGp[static_cast<int>(GeneralPurposeRegisters::Count)];

        // Floating point registers
        RegisterFP regFp[static_cast<int>(FloatingPointRegisters::Count)];

        // Function Registers
        RegisterInt regAp[static_cast<int>(ArgumentRegisters::Count)];
        RegisterInt regRp;

        // Stack Pointer
        RegisterInt regSp;

        // Execution Flags
        WispExecutionFlags eflags;
    };
}