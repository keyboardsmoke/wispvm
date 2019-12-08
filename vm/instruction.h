#pragma once

namespace wisp
{
    class Vm;
    class State;

    class Instruction
    {
    public:
        static Instruction* CreateInstruction(uint8* pc);

        virtual VmError Execute(Vm* vm, State* state) = 0;
    };
}