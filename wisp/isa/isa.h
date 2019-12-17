#pragma once

#include "vm/isa.h"

namespace wisp
{
    class WispContext;

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

    class WispISA : public vmcore::ISA
    {
    public:
        vmcore::VmError ExecuteInstruction(vmcore::Vm* vm) override;

        // Read an address which is a delta from the instruction PC using the provided encoding
        vmcore::VmError GetRelativeAddressDestinationFromPc(vmcore::Vm* vm, WispContext* context, uint8 encoding, uint64 pc, uint64* addr);

        template<typename T>
        T ReadArgument(vmcore::Vm* vm)
        {
            T ret = *reinterpret_cast<T*>(vm->GetMemory()->GetPhysicalMemory() + vm->GetContext()->regPc.Get());
            vm->GetContext()->regPc.Advance(sizeof(T));
            return ret;
        }
    };
}