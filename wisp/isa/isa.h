#pragma once

#include "vm/isa.h"
#include "vm/vm.h"

namespace wisp
{

    class WispContext;
    class WispISA;

    typedef vmcore::VmError(*isa_fn)(WispISA*, vmcore::Vm*, WispContext*, uint64);

    enum class ConditionCode
    {
        Overflow,                   // OverflowFlag == 1 (if overflow)
        NotOverflow,                // OverflowFlag == 0 (if not overflow)

        LessThan,                   // CarryFlag == 1 (below/not above or equal/carry)
        GreaterThan,                // CarryFlag == 0 && ZeroFlag == 0 (if not below or equal/above)
        GreaterThanOrEqual,         // CarryFlag == 0 (not below/above or equal/not carry)
        LessThanOrEqual,            // CarryFlag == 1 || ZeroFlag == 1 (below or equal/not above)

        Zero,                       // ZeroFlag == 1 (zero/equal)
        NotZero,                    // ZeroFlag == 0 (not zero/not equal)
        Signed,                     // SignFlag == 1 (if sign)
        NotSigned,                  // SignFlag == 0 (if not sign)
        Parity,                     // ParityFlag == 1 (if parity/parity even)
        NotParity,                  // ParityFlag == 0 (if not parity/parity odd)
    };

    enum class InstructionCodes
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
    };

    class WispISAModule
    {
    public:
        virtual vmcore::VmError Create(isa_fn* functionList) = 0;
    };

    class WispISA : public vmcore::ISA
    {
    public:
        vmcore::VmError RegisterModule(WispISAModule* pModule)
        {
            return pModule->Create(m_isaFunctions);
        }

        vmcore::VmError Initialize() override;
        vmcore::VmError ExecuteInstruction(vmcore::Vm* vm) override;

    private:
        isa_fn m_isaFunctions[0xFF]; // Maximum
    };
}