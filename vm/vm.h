#pragma once

#include "shared/types.h"
#include "value.h"
#include "state.h"

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
        CorruptBlob,
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

    struct FunctionPrototype
    {
        ValueType returnType;
        std::vector<ValueType> arguments;
    };

    class Vm
    {
    public:
        Vm() : m_state() {}

        static const char* GetErrorString(VmError error);

        VmError BindGlobal(Value* value, const std::string& name);
        VmError BindFunction(void* functionPointer, const std::string& functionName, const ValueType& returnType const std::vector<ValueType>& argumentTypes);
        VmError ExecuteProgram(void* program, uint32 size);

    private:
        VmError ExecuteState();
        VmError ExecuteInstruction(uint8* pc);

        State m_state;
    };
}
