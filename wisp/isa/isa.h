#pragma once

#include "vm/isa.h"
#include "vm/vm.h"

namespace wisp
{
    class WispContext;
    class WispISAModule;
    class WispISA;

    typedef vmcore::VmError(*isa_fn)(WispISA*, WispISAModule*, vmcore::Vm*, WispContext*, uint64);

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
        MoveConstant,
        MoveRelative,
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
        Not,

        // SystemCall takes an arbitrary amount of operands depending on the call
        // Some system calls are implemented by wisp, while others are a result of
        // native function expansion by the user binding against wisp.
        SystemCall,

        // Below this line are functions which enable non-basic types to function
        // such as: table, array, they're a little weirder than what you'd expect from bytecode
        // Each non-basic type is represented by an integer, which is an index into the complexType pool

        // Strings
        StringCreate,
        StringSet,
        StringGet,
        StringClear,
        StringIsEmpty,
        StringAppend,

        // Tables
        TableCreate,
        TableGet,
        TableClear,
        TableIsEmpty,
        TableSetKeyValue,
        TableGetKeyValue,
        TableGetSize,
        TableDestroy,

        // Arrays
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
    };

    class WispISAModule
    {
    public:
        virtual vmcore::VmError Create(std::unordered_map<InstructionCodes, isa_fn>& functionList) = 0;
    };

    struct ISAHandlerEntry
    {
        WispISAModule* module;
        std::unordered_map<InstructionCodes, isa_fn> registration;
    };

    class WispISA : public vmcore::ISA
    {
    public:
        struct InstructionHandler
        {
            WispISAModule* mod;
            isa_fn func;
        };

        vmcore::VmError RegisterModule(WispISAModule* pModule)
        {
            std::unordered_map<InstructionCodes, isa_fn> moduleMap;
            vmcore::VmError err = pModule->Create(moduleMap);
            if (err != vmcore::VmError::OK)
                return err;

            // Override registrations
            // This means ISA modules that register handlers
            // on top of existing handlers will override
            for (auto mm : moduleMap)
            {
                m_isaFunctions[mm.first].mod = pModule;
                m_isaFunctions[mm.first].func = mm.second;
            }

            return vmcore::VmError::OK;
        }

        vmcore::VmError Initialize() override;
        vmcore::VmError ExecuteInstruction(vmcore::Vm* vm) override;

    private:
        std::unordered_map<InstructionCodes, InstructionHandler> m_isaFunctions;
    };
}