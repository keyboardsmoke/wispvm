#include "vm.h"
#include "shared/random.h"
#include "shared/decoder.h"
#include "shared/hash/crc32.h"
#include "instruction.h"

using namespace wisp;

const char* Vm::GetErrorString(VmError error)
{
    switch(error)
    {
        case VmError::OK:
            return "None";
        case VmError::InvalidOpcode:
            return "Invalid Opcode";
        default:
            return "Unknown";
    }
}

VmError Vm::BindGlobal(Value* value, const std::string& name)
{
    return VmError::OK;
}

VmError Vm::BindFunction(void* functionPointer, const std::string& functionName, const ValueType& returnType const std::vector<ValueType>& argumentTypes)
{
    // We will utilize the function signature for type enforcement during runtime

    return VmError::OK;
}

VmError Vm::ExecuteProgram(void *program, uint32 size)
{
    if (program == nullptr || size < sizeof(ProgramHeader))
        return VmError::InvalidArguments;

    ProgramHeader* header = reinterpret_cast<ProgramHeader*>(program);

    const uint32 magic = Decoder::Decode(header->magic, header->seed);

    if (magic != ProgramHeader::WISP_MAGIC)
        return VmError::InvalidMagic;

    const uint32 providedCrc = Decoder::Decode(header->bytecodeCrc, header->seed);

    uint8* pProgram = reinterpret_cast<uint8*>(program);
    uint8* pProgramStart = pProgram + sizeof(ProgramHeader);
    uint8* pProgramEnd = pProgramStart + size;
    uint32 programSize = size - sizeof(ProgramHeader);

    const uint32 calculatedCrc = hash::crc32(pProgramStart, programSize);

    if (calculatedCrc != providedCrc)
        return VmError::CorruptBlob;

    const uint32 decodedEp = Decoder::Decode(header->ep, header->seed);

    uint8* pEntryPoint = pProgramStart + decodedEp;

    if (pEntryPoint >= pProgramEnd)
        return VmError::CorruptBlob;

    m_state.programBase = pProgramStart;
    m_state.programSize = programSize;

    PointerValue* ep = new PointerValue(header->ep);

    m_state.regPc.SetValue(ep);

    // Start execution

    return VmError::OK;
}

VmError Vm::ExecuteState()
{
    if (m_state.programBase == nullptr || m_state.programSize == 0)
    {
        return VmError::InvalidProgramState;
    }

    PointerValue* pcValue = dynamic_cast<PointerValue*>(m_state.regPc.GetValue());

    if (pcValue == nullptr)
    {
        // Invalid PC register value

        return VmError::RegisterMismatch;
    }

    uint8* pc = m_state.programBase + pcValue->GetOffset();

    // Execute Instruction at PC

    // You should ideally be calling ExecuteState in a while loop until it hits VmError::EndOfProgram
    // Which is triggered when the ep returns and there is nothing left on the stack to return to.

    return ExecuteInstruction(pc);
}

VmError Vm::ExecuteInstruction(uint8* pc)
{
    // We want to get the instruction type from PC
    // Which should involve going through the instruction class
    // Then, there should just be an execute virtual call which updates value/register/etc state.
    Instruction* inst = Instruction::CreateInstruction(pc);

    if (inst == nullptr)
    {
        // Invalid Instruction
        return VmError::InvalidInstruction;
    }

    return inst->Execute(this, &m_state);
}