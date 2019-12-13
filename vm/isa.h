#pragma once

namespace wisp
{
    class Vm;
    enum class VmError;

    class ISA
    {
    public:
        virtual VmError ExecuteInstruction(Vm* vm) = 0;
    };
}