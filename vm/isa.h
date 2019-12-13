#pragma once

namespace vmcore
{
    class Vm;
    enum class VmError;

    class ISA
    {
    public:
        virtual VmError ExecuteInstruction(Vm* vm) = 0;
    };
}