#include "vm.h"
#include "shared/random.h"
#include "shared/decoder.h"
#include "shared/hash/crc32.h"

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

VmError Vm::BindFunction(void* functionPointer, const char* functionName, const ValueType& returnType const std::vector<ValueType>& argumentTypes)
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

    // Start executing at the entrypoint :)

    return VmError::OK;
}