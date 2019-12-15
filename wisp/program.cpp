#include "shared/types.h"
#include "shared/hash/crc32.h"
#include "shared/decoder.h"
#include "vm/vm.h"
#include "program.h"

using namespace vmcore;
using namespace wisp;

std::vector<uint8> Program::CreateProgram(std::vector<uint8>& byteCode)
{
    Program::ProgramHeader header;
    header.seed = 0; // When no seed is set, nothing is really encoded/randomized
    header.magic = Program::ProgramHeader::WISP_MAGIC;
    header.ep = 0;
    header.bytecodeCrc = wisp::hash::crc32(byteCode.data(), static_cast<uint32>(byteCode.size()));

    uint8* headerBytes = reinterpret_cast<uint8*>(&header);

    std::vector<uint8> prog(headerBytes, headerBytes + sizeof(Program::ProgramHeader));

    prog.insert(prog.end(), byteCode.begin(), byteCode.end());

    return prog;
}

VmError Program::ExecuteProgram(Vm* vm, void *program, uint32 size)
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

    return vm->Execute(header->ep);
}