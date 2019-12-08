#pragma once

#include "shared/types.h"
#include "value.h"

// The "Vm" object is an instance of the virtual machine

namespace wisp
{
    enum class VmError
    {
        OK = 0,
        InvalidArguments,
        InvalidMagic,
        InvalidOpcode,
        CorruptBlob,
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
        static const char* GetErrorString(VmError error);

        VmError BindFunction(void* functionPointer, const char* functionName, const ValueType& returnType const std::vector<ValueType>& argumentTypes);
        VmError ExecuteProgram(void* program, uint32 size);

    private:
        //
    };
}
