#include "shared/types.h"
#include "vm/modules/memory.h"
#include "vm/vm.h"
#include "vm/isa.h"
#include "shared/hash/crc32.h"
#include "context.h"
#include "isa/isa.h"
#include "program.h"

int main(int argc, char** argv)
{
    UNREFERENCED_PARAMETER(argc);
    UNREFERENCED_PARAMETER(argv);

    // Basic memory module, we can make one that expands or even uses a different pool from main program memory
    // but I'm lazy, so no.
    uint64 memorySize = 16000 * 1000; // 16MB
    vmcore::MemoryModule ram(memorySize);
	wisp::WispContext context;
    wisp::WispISA isa;
    vmcore::Vm instance(&context, &ram, &isa);

    // bytecode?

    // wisp::Program::CreateProgram()

    return 0;
}