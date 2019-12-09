#pragma once

#include "shared/types.h"
#include "register.h"
#include "stack.h"

namespace wisp
{
    enum class VmError;

    struct StateFlags
    {
        StateFlags() :
            carry(0), parity(0),
            adjust(0), zero(0),
            sign(0), direction(0),
            overflow(0), reserved(0) {}

        uint8 carry : 1;
        uint8 parity : 1;
        uint8 adjust : 1;
        uint8 zero : 1;
        uint8 sign : 1;
        uint8 direction : 1;
        uint8 overflow : 1;
        uint8 reserved : 1;
    };
    static_assert(sizeof(StateFlags) == sizeof(uint8), "Invalid size for wisp::Flags");

    class State
    {
    public:
        static const uint32 ArgRegisterCount = 16;
        static const uint32 GpRegisterCount = 32;

        State() = delete;
        State(uint64 stackReserveSize) :
                programBase(nullptr),
                programSize(0),
                flags(),
                stack(stackReserveSize)
        {

        }

        VmError Release();

        uint8* programBase;
        uint32 programSize;

        // Argument Registers (a0 - a15)
        // Argument registers are cleared after every RETURN instruction
        // That way, we can know how many arguments were set for a pending function call
        Register regArguments[ArgRegisterCount];

        // Return value register (rv, return value)
        Register regReturn;

        // General Purpose Registers (r0 - r31)
        Register regGeneral[GpRegisterCount];

        // PC Register (pc, program counter)
        Register regPc;

        // SP Register (sp, stack pointer)
        Register regSp;

        //
        StateFlags flags;

        // The stack management class
        Stack stack;
    };
}