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
        case VmError::InvalidArguments:
            return "Invalid Arguments";
        case VmError::InvalidMagic:
            return "Invalid Program Signature";
        case VmError::InvalidOpcode:
            return "Invalid Opcode";
        case VmError::InvalidProgramState:
            return "Invalid Program State";
        case VmError::InvalidInstruction:
            return "Invalid Instruction";
        case VmError::InvalidEntryPoint:
            return "Invalid EntryPoint";
        case VmError::ProgramCrcMismatch:
            return "Program CRC mismatch";
        case VmError::CorruptProgram:
            return "Invalid program, possible corruption";
        case VmError::RegisterMismatch:
            return "Register type mismatch detected";
        case VmError::EndOfProgram:
            return "Program ended";
    }

    return "Unknown";
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
        return VmError::ProgramCrcMismatch;

    const uint32 decodedEp = Decoder::Decode(header->ep, header->seed);

    uint8* pEntryPoint = pProgramStart + decodedEp;

    if (pEntryPoint < pProgramStart || pEntryPoint >= pProgramEnd)
        return VmError::InvalidEntryPoint;

    m_state.programBase = pProgramStart;
    m_state.programSize = programSize;

    PointerValue* ep = new PointerValue(header->ep);

    m_state.regPc.SetValue(ep);

    // Start execution
    VmError err = VmError::OK;

    while (true)
    {
        err = ExecuteState();

        if (err == VmError::OK)
        {
            continue;
        }

        if (err == VmError::EndOfProgram)
        {
            break;
        }

        return err;
    }

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

    uint8* newPc = pc;
    VmError err = ExecuteInstruction(&newPc);
    if (err != VmError::OK)
        return err;

    uint32 dist = newPc - pc;

    pcValue->Increment(dist);

    return VmError::OK;
}

VmError Vm::ExecuteInstruction(uint8** pc)
{
    uint8 instId = *((*pc)++);

    return m_instList->Execute(this, &m_state, pc, instId);
}