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

    class WispContext : public vmcore::Context
    {
    public:
        // General purpose registers
        Register regGp[static_cast<int>(GeneralPurposeRegisters::Count)];

        // Stack Pointer
        Register regSp;

        // Execution Flags
        WispExecutionFlags eflags;
    };
}