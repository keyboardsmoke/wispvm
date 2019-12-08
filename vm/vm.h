#pragma once

#include "shared/types.h"
#include "value.h"
#include "state.h"
#include "native.h"
#include "instruction.h"

// The "Vm" object is an instance of the virtual machine

namespace wisp
{
    enum class VmError
    {
        OK = 0,
        InvalidArguments,
        InvalidMagic,
        InvalidOpcode,
        InvalidProgramState,
        InvalidInstruction,
        InvalidEntryPoint,
        ProgramCrcMismatch,
        CorruptProgram,
        RegisterMismatch,

        // Happens when the EP returns
        EndOfProgram
    };

    struct ProgramHeader
    {
        static const uint32 WISP_MAGIC = 0x77697370; // 'wisp'

        // Seed dictates how the following values are encoded
        // That way, when we use the seed to decode magic
        // We can still ensure integrity without having an obvious static magic
        uint64 seed;
        uint32 magic;
        uint32 ep;
        uint32 bytecodeCrc;
    };

    class Vm
    {
    public:
        Vm() = delete;
        Vm(NativeList* nativeList, InstructionList* instList) :
                m_state(), m_nativeList(nativeList), m_instList(instList) {}

        static const char* GetErrorString(VmError error);

        VmError ExecuteProgram(void* program, uint32 size);

        NativeList* GetNativeList()
        {
            return m_nativeList;
        }

        InstructionList* GetInstructionList()
        {
            return m_instList;
        }

    private:
        VmError ExecuteState();
        VmError ExecuteInstruction(uint8** pc);

        State m_state;
        NativeList* m_nativeList;
        InstructionList* m_instList;
    };
}
