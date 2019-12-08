#include "doctest/doctest.h"
#include "vm/vm.h"

static wisp::VmError StoreRegisterInt(wisp::Vm* vm, wisp::State* state)
{
    wisp::uint8 regIndex = *vm->GetProgramCounterData();
    vm->AdvanceProgramCounter(sizeof(wisp::uint8));
    wisp::uint8 constant = *vm->GetProgramCounterData();
    vm->AdvanceProgramCounter(sizeof(wisp::uint8));

    wisp::IntegerValue* valueInt = dynamic_cast<wisp::IntegerValue*>(state->regGeneral[regIndex].GetValue());

    if (valueInt == nullptr)
    {
        state->regGeneral[regIndex].SetValue(new wisp::IntegerValue(constant));
    }
    else
    {
        valueInt->SetInt(constant);
    }

    return wisp::VmError::OK;
}

static wisp::VmError AddRegisterToRegister(wisp::Vm* vm, wisp::State* state)
{
    wisp::uint8 regIndex0 = *vm->GetProgramCounterData();
    vm->AdvanceProgramCounter(sizeof(wisp::uint8));
    wisp::uint8 regIndex1 = *vm->GetProgramCounterData();
    vm->AdvanceProgramCounter(sizeof(wisp::uint8));

    wisp::IntegerValue* reg0 = dynamic_cast<wisp::IntegerValue*>(state->regGeneral[regIndex0].GetValue());
    wisp::IntegerValue* reg1 = dynamic_cast<wisp::IntegerValue*>(state->regGeneral[regIndex1].GetValue());

    if (reg0 == nullptr || reg1 == nullptr)
    {
        return wisp::VmError::InvalidInstruction;
    }

    wisp::uint64 val = reg0->GetInt<wisp::uint8>() + reg1->GetInt<wisp::uint8>();

    reg0->SetInt<wisp::uint64>(val);

    return wisp::VmError::OK;
}

static wisp::VmError EndProgram(wisp::Vm* vm, wisp::State* state)
{
    return wisp::VmError ::EndOfProgram;
}

typedef std::vector<wisp::uint8> ProgramCode;

TEST_CASE("Simple VM")
{
    // Not going to do any native stuff here
    wisp::NativeList modList;

    // Just addition!
    wisp::InstructionList instList;
    wisp::uint32 StoreRegisterIntIndex = instList.AddInstruction(StoreRegisterInt);
    wisp::uint32 AddRegisterToRegisterIndex = instList.AddInstruction(AddRegisterToRegister);
    wisp::uint32 EndProgramIndex = instList.AddInstruction(EndProgram);

    wisp::uint8 reg0 = 0;
    wisp::uint8 reg1 = 1;

    ProgramCode programCode;

    programCode.push_back(static_cast<wisp::uint8>(StoreRegisterIntIndex));
    programCode.push_back(reg0);
    programCode.push_back(1);

    programCode.push_back(static_cast<wisp::uint8>(StoreRegisterIntIndex));
    programCode.push_back(reg1);
    programCode.push_back(10);

    programCode.push_back(static_cast<wisp::uint8>(AddRegisterToRegisterIndex));
    programCode.push_back(reg0);
    programCode.push_back(reg1);

    programCode.push_back(static_cast<wisp::uint8>(EndProgramIndex)); // END_PROGRAM

    ProgramCode prog = wisp::Vm::CreateProgram(programCode);

    wisp::Vm vm(&modList, &instList);
    wisp::VmError err = vm.ExecuteProgram(prog.data(), static_cast<wisp::uint32>(prog.size()));
    REQUIRE(err == wisp::VmError::OK);
    REQUIRE(dynamic_cast<wisp::IntegerValue*>(vm.GetState()->regGeneral[reg0].GetValue())->GetInt<wisp::uint8>() == 11);
}