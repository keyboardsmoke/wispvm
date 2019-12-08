#include "doctest/doctest.h"
#include "vm/vm.h"

void MyHelloWorld(wisp::Vm* vm, wisp::State* state)
{
    printf("Hello, World!\n");
    printf("Small Change.\n");

    // state->regGeneral[0].SetValue(new wisp::IntegerValue(100));

    return;
}

wisp::VmError Inst0(wisp::Vm* vm, wisp::State* state)
{
    // This instruction calls a native by ID!

    wisp::uint8 index = *vm->GetProgramCounterData();

    vm->AdvanceProgramCounter(sizeof(wisp::uint8));

    auto natives = vm->GetNativeList()->GetNatives();

    if (index < natives.size())
    {
        natives[index](vm, state);
        return wisp::VmError::OK;
    }

    return wisp::VmError::InvalidInstruction;
}

wisp::VmError Inst1(wisp::Vm* vm, wisp::State* state)
{
    // terminate!

    return wisp::VmError::EndOfProgram;
}

TEST_CASE("Simple VM")
{
    std::vector<wisp::uint8> code;
    code.push_back(0); // CALL_NATIVE
    code.push_back(0); // index 0
    code.push_back(1); // END_PROGRAM

    std::vector<wisp::uint8> prog = wisp::Vm::CreateProgram(code);

    wisp::NativeList modList;
    modList.AddNative(MyHelloWorld);

    wisp::InstructionList instList;
    instList.AddInstruction(Inst0);
    instList.AddInstruction(Inst1);

    wisp::Vm vm(&modList, &instList);
    wisp::VmError err = vm.ExecuteProgram(prog.data(), prog.size());
    REQUIRE(err == wisp::VmError::OK);
}