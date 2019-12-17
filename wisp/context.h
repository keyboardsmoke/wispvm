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

    class WispContext : public vmcore::Context
    {
    public:
        uint8 GetRegisterId(Register& reg);

        // General purpose registers
        Register regGeneral[32];

        // Function Registers
        Register regArgument[16];
        Register regReturn;

        // Stack Pointer
        Register regSp;

        // Execution Flags
        WispExecutionFlags eflags;
    };
}