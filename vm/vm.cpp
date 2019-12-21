#include "vm.h"
#include "shared/random.h"
#include "shared/decoder.h"
#include "shared/hash/crc32.h"
#include "isa.h"

using namespace vmcore;

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

VmError Vm::Initialize()
{
    auto err = m_isa->Initialize();
    if (err != VmError::OK)
        return err;

    m_initialized = true;

    return VmError::OK;
}

VmError Vm::Execute(uint64 offset)
{
    // Initialize if the user has not yet initialized.
    // The user can control when Initialization happens by calling it themselves if they want.
    if (m_initialized == false)
    {
        auto err = Initialize();
        if (err != VmError::OK)
            return err;
    }

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
    // TODO: We want to check to see if the tail can execute too, don't we?...

    if (!m_memory->CanExecute(m_context->regPc.Get(), 1u))
    {
        return VmError::AccessViolation;
    }

    return m_isa->ExecuteInstruction(this);
}