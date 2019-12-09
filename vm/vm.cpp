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

std::vector<wisp::uint8> Vm::CreateProgram(std::vector<wisp::uint8>& byteCode)
{
    wisp::ProgramHeader header;
    header.seed = 0; // When no seed is set, nothing is really encoded/randomized
    header.magic = wisp::ProgramHeader::WISP_MAGIC;
    header.ep = 0;
    header.bytecodeCrc = wisp::hash::crc32(byteCode.data(), static_cast<wisp::uint32>(byteCode.size()));

    wisp::uint8* headerBytes = reinterpret_cast<wisp::uint8*>(&header);

    std::vector<wisp::uint8> prog(headerBytes, headerBytes + sizeof(wisp::ProgramHeader));

    prog.insert(prog.end(), byteCode.begin(), byteCode.end());

    return prog;
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

    PointerValue* sp = new PointerValue(0);

    m_state.regSp.SetValue(sp);

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

    // Execute Instruction at PC

    // You should ideally be calling ExecuteState in a while loop until it hits VmError::EndOfProgram
    // Which is triggered when the ep returns and there is nothing left on the stack to return to.
    VmError err = ExecuteInstruction(pcValue);
    if (err != VmError::OK)
        return err;

    return VmError::OK;
}

VmError Vm::ExecuteInstruction(PointerValue* pc)
{
    uint8 instId = *(m_state.programBase + pc->GetOffset());

    // Advance PC so that the handlers start off in the right place.
    VmError err = AdvanceProgramCounter(sizeof(uint8));
    if (err != VmError::OK)
        return err;

    return m_instList->Execute(this, &m_state, instId);
}

VmError Vm::AdvanceProgramCounter(uint32 size)
{
    wisp::PointerValue* pcValue = dynamic_cast<wisp::PointerValue*>(m_state.regPc.GetValue());
    if (pcValue != nullptr)
    {
        pcValue->Increment(size);
        return VmError::OK;
    }

    return VmError::RegisterMismatch;
}

uint8* Vm::GetProgramCounterData()
{
    wisp::PointerValue* pcValue = dynamic_cast<wisp::PointerValue*>(m_state.regPc.GetValue());
    if (pcValue != nullptr)
    {
        return m_state.programBase + pcValue->GetOffset();
    }

    return nullptr;
}