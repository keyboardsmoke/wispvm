#pragma once

#include "vm/context.h"
#include "register.h"

namespace wisp
{
    class WispContext : public Context
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
    };
}