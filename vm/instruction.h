#pragma once

namespace wisp
{
    class Vm;
    class State;
    enum class VmError;

    // Every instruction in our instruction set operates on register values or constant values
    // and every instruction is represented by a native
    // when the VM reads the instruction ID (first byte)
    // it passes the PC to the instruction handler which decides how to interpret the data it's been given
    typedef std::function<VmError(Vm*, State*)> InstructionDefinition;

    class InstructionList
    {
    public:
        void AddInstruction(InstructionDefinition def)
        {
            m_instructions.emplace_back(def);
        }

        VmError Execute(Vm* vm, State* state, uint8 id);

    private:
        std::vector<InstructionDefinition> m_instructions;
    };
}