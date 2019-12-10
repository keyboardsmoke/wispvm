#include "doctest/doctest.h"
#include "vm/vm.h"

// Reserved Register Numbers
const wisp::uint8 sp = 0xff;
const wisp::uint8 pc = 0xfe;
const wisp::uint8 retval = 0xfd;

// This function has to exist to emulate indices for reserved registers (sp, pc, etc)
wisp::Register& TranslateRegisterNumber(wisp::uint8 num, wisp::Vm* vm, bool argument = false)
{
    if (num == sp)
    {
        return vm->GetState()->regSp;
    }
    else if (num == pc)
    {
        return vm->GetState()->regPc;
    }
    else if (num == retval)
    {
        return vm->GetState()->regReturn;
    }
    else if (argument)
    {
        return vm->GetState()->regArguments[num];
    }

    return vm->GetState()->regGeneral[num];
}

bool IsRegisterStackPointer(wisp::uint8 num)
{
    return (num == sp);
}

static wisp::VmError StoreRegisterUInt8(wisp::Vm* vm, wisp::State* state)
{
    wisp::uint8 regIndex = vm->ReadArgument<wisp::uint8>();
    wisp::uint8 constant = vm->ReadArgument<wisp::uint8>();

    wisp::Register& reg = TranslateRegisterNumber(regIndex, vm);

    reg.SetUInt8(constant);

    return wisp::VmError::OK;
}

static wisp::VmError MoveRegisterToRegister(wisp::Vm* vm, wisp::State* state)
{
    wisp::uint8 regIndex0 = vm->ReadArgument<wisp::uint8>();
    wisp::uint8 regIndex1 = vm->ReadArgument<wisp::uint8>();

    wisp::Register& reg0 = TranslateRegisterNumber(regIndex0, vm);
    wisp::Register& reg1 = TranslateRegisterNumber(regIndex1, vm);

    reg0.CopyValue(reg1);

    return wisp::VmError::OK;
}

static wisp::VmError AddRegisterToRegister(wisp::Vm* vm, wisp::State* state)
{
    wisp::uint8 regIndex0 = vm->ReadArgument<wisp::uint8>();
    wisp::uint8 regIndex1 = vm->ReadArgument<wisp::uint8>();

    wisp::Register& reg0 = TranslateRegisterNumber(regIndex0, vm);
    wisp::Register& reg1 = TranslateRegisterNumber(regIndex1, vm);

    if (!reg0.HasValue() || !reg1.HasValue())
    {
        return wisp::VmError::InvalidInstruction;
    }

    return reg0.AddRegister(reg1);
}

static wisp::VmError AndRegisterToRegister(wisp::Vm* vm, wisp::State* state)
{
	wisp::uint8 regIndex0 = vm->ReadArgument<wisp::uint8>();
	wisp::uint8 regIndex1 = vm->ReadArgument<wisp::uint8>();

	wisp::Register& reg0 = TranslateRegisterNumber(regIndex0, vm);
	wisp::Register& reg1 = TranslateRegisterNumber(regIndex1, vm);

	if (!reg0.HasValue() || !reg1.HasValue())
	{
		return wisp::VmError::InvalidInstruction;
	}

	return reg0.AndRegister(reg1);
}

static wisp::VmError AddNumToRegister(wisp::Vm* vm, wisp::State* state)
{
    wisp::uint8 regIndex = vm->ReadArgument<wisp::uint8>();
    wisp::uint8 constant = vm->ReadArgument<wisp::uint8>();

    wisp::Register& reg = TranslateRegisterNumber(regIndex, vm);

    // Special handling for the stack pointer
    if (IsRegisterStackPointer(regIndex))
    {
        // Request stack size
        state->stack.Request(constant);
    }

    if (reg.IsUnsignedInteger())
    {
        reg.AddUnsignedInteger(constant);
    }
    else if (reg.IsSignedInteger())
    {
        // TODO: Is this even correct? lol
        reg.AddSignedInteger(static_cast<wisp::int64>(constant));
    }
    else if (reg.IsPointer())
    {
        reg.SetPointer(reg.GetPointerOffset() + constant);
    }
    else
    {
        return wisp::VmError::InvalidInstruction;
    }

    return wisp::VmError::OK;
}

static wisp::VmError SubtractNumFromRegister(wisp::Vm* vm, wisp::State* state)
{
    wisp::uint8 regIndex = vm->ReadArgument<wisp::uint8>();
    wisp::uint8 constant = vm->ReadArgument<wisp::uint8>();

    wisp::Register& reg = TranslateRegisterNumber(regIndex, vm);

    if (reg.IsUnsignedInteger())
    {
        reg.SubUnsignedInteger(constant);
    }
    else if (reg.IsSignedInteger())
    {
        // TODO: Is this even correct? lol
        reg.SubSignedInteger(static_cast<wisp::int64>(constant));
    }
    else if (reg.IsPointer())
    {
        reg.SetPointer(reg.GetPointerOffset() - constant);
    }
    else
    {
        return wisp::VmError::InvalidInstruction;
    }

    return wisp::VmError::OK;
}

static void PrintRegister(const char* name, wisp::Register& reg)
{
    printf("Register %s [type: ", name);

    if (reg.IsPointer())
    {
        printf("pointer][value: 0x%ld", reg.GetPointerOffset());
    }
    else if (reg.IsUnsignedInteger())
    {
        printf("uint][value: 0x%ld", reg.GetInteger<wisp::uint64>());
    }
    else if (reg.IsSignedInteger())
    {
        printf("int][value: %ld", reg.GetInteger<wisp::int64>());
    }
    else
    {
        printf("Unknown");
    }

    printf("]\n");
}

static wisp::VmError PrintContext(wisp::Vm* vm, wisp::State* state)
{
    PrintRegister("pc", state->regPc);
    PrintRegister("sp", state->regSp);

    return wisp::VmError::OK;
}

static wisp::VmError EndProgram(wisp::Vm* vm, wisp::State* state)
{
    return wisp::VmError::EndOfProgram;
}

typedef std::vector<wisp::uint8> ProgramCode;

TEST_CASE("Simple VM")
{
    // Not going to do any native stuff here
    wisp::NativeList modList;

    // Just addition!
    wisp::InstructionList instList;
    const wisp::uint8 STORE_UINT8 = static_cast<wisp::uint8>(instList.AddInstruction(StoreRegisterUInt8));
    const wisp::uint8 MOVE_REG_TO_REG = static_cast<wisp::uint8>(instList.AddInstruction(MoveRegisterToRegister));
    const wisp::uint8 ADD_REG_TO_REG = static_cast<wisp::uint8>(instList.AddInstruction(AddRegisterToRegister));
    const wisp::uint8 AND_REG_TO_REG = static_cast<wisp::uint8>(instList.AddInstruction(AndRegisterToRegister));
    const wisp::uint8 ADD_NUM_REG = static_cast<wisp::uint8>(instList.AddInstruction(AddNumToRegister));
    const wisp::uint8 SUB_NUM_REG = static_cast<wisp::uint8>(instList.AddInstruction(SubtractNumFromRegister));
    const wisp::uint8 PRINT_CONTEXT = static_cast<wisp::uint8>(instList.AddInstruction(PrintContext));
    const wisp::uint8 END_PROGRAM = static_cast<wisp::uint8>(instList.AddInstruction(EndProgram));

    const wisp::uint8 reg0 = 0;
    const wisp::uint8 reg1 = 1;
    const wisp::uint8 reg2 = 2;
    const wisp::uint8 reg3 = 3;
    const wisp::uint8 reg4 = 4;

    // 0x06 & 0x1E == 0x06

    wisp::uint8 programCode[] =
    {
        ADD_NUM_REG, sp, 0x10, // our stack grows down...
        // PRINT_CONTEXT,
        STORE_UINT8, reg0, 1,
        STORE_UINT8, reg1, 10,
        STORE_UINT8, reg3, 0x06,
        STORE_UINT8, reg4, 0x1E,
        AND_REG_TO_REG, reg3, reg4,
        MOVE_REG_TO_REG, reg2, reg1,
        ADD_REG_TO_REG, reg0, reg1,
        SUB_NUM_REG, sp, 0x10,
        // PRINT_CONTEXT,
        END_PROGRAM
    };

    ProgramCode programCodeVec(programCode, programCode + sizeof(programCode));
    ProgramCode prog = wisp::Vm::CreateProgram(programCodeVec);

    wisp::Vm vm(&modList, &instList, 0x1000);
    wisp::VmError err = vm.ExecuteProgram(prog.data(), static_cast<wisp::uint32>(prog.size()));
    REQUIRE(err == wisp::VmError::OK);

    REQUIRE(vm.GetState()->regGeneral[reg0].IsUnsignedInteger());
    REQUIRE(vm.GetState()->regGeneral[reg2].IsUnsignedInteger());
    REQUIRE(vm.GetState()->regGeneral[reg3].IsUnsignedInteger());

    REQUIRE(vm.GetState()->regGeneral[reg0].GetInteger<wisp::uint8>() == 11);
    REQUIRE(vm.GetState()->regGeneral[reg2].GetInteger<wisp::uint8>() == 10);
    REQUIRE(vm.GetState()->regGeneral[reg3].GetInteger<wisp::uint8>() == 0x06);

    vm.GetState()->Release();
}