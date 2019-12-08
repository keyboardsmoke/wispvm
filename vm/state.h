#pragma once

#include "shared/types.h"
#include "register.h"

namespace wisp
{
    struct StateFlags
    {
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
        uint8* programBase;
        uint32 programSize;

        // Argument Registers (a0 - a15)
        // Argument registers are cleared after every RETURN instruction
        // That way, we can know how many arguments were set for a pending function call
        Register regArguments[16];

        // Return value register (retval)
        Register regReturn;

        // General Purpose Registers (r0 - r31)
        Register regGeneral[32];

        // PC Register (pc)
        Register regPc;

        StateFlags flags;
    };
}