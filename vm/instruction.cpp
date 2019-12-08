#include "vm.h"
#include "instruction.h"

using namespace wisp;

VmError InstructionList::Execute(Vm* vm, State* state, uint8 id)
{
    if (id < m_instructions.size())
    {
        return m_instructions[id](vm, state);
    }

    return VmError::InvalidOpcode;
}