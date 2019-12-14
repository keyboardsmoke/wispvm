#pragma once

#include "shared/types.h"
#include "context.h"
#include "isa.h"
#include "modules/memory.h"

// The "Vm" object is an instance of the virtual machine

namespace vmcore
{
    enum class VmError
    {
        OK = 0,
        InvalidArguments,
        InvalidMagic,
        InvalidContext,
        InvalidOpcode,
        InvalidProgramState,
        InvalidInstruction,
        InvalidEntryPoint,
        ProgramCrcMismatch,
        CorruptProgram,
        RegisterMismatch,

        // The VM will immediately halt execution
        HaltExecution
    };

    class Vm
    {
    public:
        Vm() = delete;
        Vm(Context* context, MemoryModule* memory, ISA* isa) :
			m_context(context), m_memory(memory), m_isa(isa) {}

        static const char* GetErrorString(VmError error);

		VmError Execute(uint64 offset);

		Context* GetContext()
        {
            return m_context;
        }

        MemoryModule* GetMemory()
        {
            return m_memory;
        }

        ISA* GetISA()
        {
            return m_isa;
        }

    private:
        VmError ExecuteState();
        VmError ExecuteInstruction();

		Context* m_context;
        MemoryModule* m_memory;
		ISA* m_isa;
    };
}
