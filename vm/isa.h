#pragma once

namespace vmcore
{
    class Vm;
    enum class VmError;

    class ISA
    {
    public:
        virtual VmError Initialize() = 0;
        virtual VmError ExecuteInstruction(Vm* vm) = 0;
    };
}