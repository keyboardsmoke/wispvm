#include "shared/types.h"
#include "vm/vm.h"
#include "vm/instruction.h"
#include "shared/hash/crc32.h"

// I figured a function like this would be easier to just do.
std::vector<wisp::uint8> CreateProgram(std::vector<wisp::uint8>& byteCode)
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

void MyHelloWorld(wisp::Vm* vm, wisp::State* state)
{
    printf("Hello, World!\n");
    return;
}

wisp::VmError Inst0(wisp::Vm* vm, wisp::State* state, wisp::uint8** pc)
{
    // This instruction calls a native by ID!
    wisp::uint8 index = *((*pc)++);

    auto natives = vm->GetNativeList()->GetNatives();

    if (index < natives.size())
    {
        natives[index](vm, state);
        return wisp::VmError::OK;
    }

    return wisp::VmError::InvalidInstruction;
}

wisp::VmError Inst1(wisp::Vm* vm, wisp::State* state, wisp::uint8** pc)
{
    // terminate!

    return wisp::VmError::EndOfProgram;
}

int main(int argc, char** argv)
{
    std::vector<wisp::uint8> code;
    code.push_back(0); // CALL_NATIVE
    code.push_back(0); // index 0
    code.push_back(1); // END_PROGRAM

    std::vector<wisp::uint8> prog = CreateProgram(code);

    // TODO: This whole module system needs to be ripped out
    // The lexer/compiler can know the string, but the VM shouldn't
    // It will also, ultimately, be index based.
    wisp::NativeList modList;
    modList.AddNative(MyHelloWorld);

    wisp::InstructionList instList;
    instList.AddInstruction(Inst0);
    instList.AddInstruction(Inst1);

    wisp::Vm vm(&modList, &instList);
    wisp::VmError err = vm.ExecuteProgram(prog.data(), prog.size());

    if (err != wisp::VmError::OK)
    {
        printf("Failed to execute program (%s)\n", wisp::Vm::GetErrorString(err));

        return 1;
    }

    return 0;
}