#include "vm.h"
#include "shared/random.h"
#include "shared/decoder.h"
#include "shared/hash/crc32.h"
#include "isa.h"

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
        case VmError::HaltExecution:
            return "Execution Finished";
    }

    return "Unknown";
}

VmError Vm::Execute(uint64 offset)
{
    m_context->regPc.GoTo(offset);

    // Start execution
    VmError err;

    while (true)
    {
        err = ExecuteState();

        if (err == VmError::OK)
        {
            continue;
        }

        if (err == VmError::HaltExecution)
        {
            break;
        }

        return err;
    }

    return VmError::OK;
}

VmError Vm::ExecuteState()
{
    VmError err = ExecuteInstruction();
    if (err != VmError::OK)
        return err;

    return VmError::OK;
}

VmError Vm::ExecuteInstruction()
{
    uint8* inst = m_memory->GetPhysicalMemory() + m_context->regPc.Get();

    return m_isa->ExecuteInstruction(this);
}

/*
VmError Vm::AdvanceProgramCounter(uint32 size)
{
    if (!m_state.regPc.HasValue() || m_state.regPc.GetType() != ValueType::Pointer)
    {
        return VmError::RegisterMismatch;
    }

    m_state.regPc.SetPointer(m_state.regPc.GetPointerOffset() + size);

    return VmError::OK;
}

uint8* Vm::GetProgramCounterData()
{
    if (!m_state.regPc.HasValue() || m_state.regPc.GetType() != ValueType::Pointer)
    {
        return nullptr;
    }

    return m_state.regPc.GetPointerFromBase(m_state.programBase);
}*/