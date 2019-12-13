#pragma once

#include "vm/isa.h"

namespace wisp
{
    enum class VmError;

    class WispISA : public ISA
    {
    public:
        VmError ExecuteInstruction(Vm* vm) override;
    };
}