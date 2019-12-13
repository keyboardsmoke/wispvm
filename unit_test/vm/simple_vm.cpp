#include "doctest/doctest.h"
#include "vm/vm.h"

// Reserved Register Numbers
const wisp::uint8 pc = 0xff;

class SimpleVmRegister
{
public:
	SimpleVmRegister() : value(0) {}

    wisp::uint8 value;
};

class SimpleVmContext : public wisp::Context
{
public:
    SimpleVmRegister regGeneral[32];
};

class SimpleVmISA : public wisp::ISA
{
public:
    SimpleVmISA(SimpleVmContext* context) : m_context(context) {}

    enum InstructionId : wisp::uint8
    {
        Store = 0,
        MoveRegToReg,
        AddRegToReg,
        AndRegToReg,
        AddNumToReg,
        SubNumFromReg,
        PrintContext,
		PrintHelloWorld,
		Jump,
        End,
    };

	wisp::VmError ExecuteInstruction(wisp::Vm* vm) override
	{
		wisp::uint64 oldPc = vm->GetContext()->regPc.Get();
		wisp::uint8 id = *(vm->GetMemory()->GetPhysicalMemory() + oldPc);
        vm->GetContext()->regPc.Advance(sizeof(wisp::uint8));
		wisp::uint64 newPc = vm->GetContext()->regPc.Get();

        switch (id)
        {
            case InstructionId::Store: return StoreNative(vm);
            case InstructionId::MoveRegToReg: return MoveRegToRegNative(vm);
            case InstructionId::AddRegToReg: return AddRegToRegNative(vm);
            case InstructionId::AndRegToReg: return AndRegToRegNative(vm);
            case InstructionId::AddNumToReg: return AddNumToRegNative(vm);
            case InstructionId::SubNumFromReg: return SubNumFromRegNative(vm);
            case InstructionId::PrintContext: return PrintContextNative(vm);
			case InstructionId::PrintHelloWorld: return PrintHelloWorldNative(vm);
			case InstructionId::Jump: return JumpNative(vm);
            case InstructionId::End: return EndNative(vm);
            default: 
			{
				printf("Invalid register index (0x%x) at (0x%I64X), New PC: 0x%I64X\n", id, oldPc, newPc);
				return wisp::VmError::InvalidInstruction;
			}
        }
    }

    wisp::VmError StoreNative(wisp::Vm* vm)
    {
        wisp::uint8 regIndex = ReadArgument<wisp::uint8>(vm);
        wisp::uint8 constant = ReadArgument<wisp::uint8>(vm);

        SimpleVmRegister& reg = TranslateRegisterNumber(regIndex, vm);

        reg.value = constant;

        return wisp::VmError::OK;
    }

    wisp::VmError MoveRegToRegNative(wisp::Vm* vm)
    {
        wisp::uint8 regIndex0 = ReadArgument<wisp::uint8>(vm);
        wisp::uint8 regIndex1 = ReadArgument<wisp::uint8>(vm);

        SimpleVmRegister& reg0 = TranslateRegisterNumber(regIndex0, vm);
        SimpleVmRegister& reg1 = TranslateRegisterNumber(regIndex1, vm);

        reg0.value = reg1.value;

        return wisp::VmError::OK;
    }

    wisp::VmError AddRegToRegNative(wisp::Vm* vm)
    {
        wisp::uint8 regIndex0 = ReadArgument<wisp::uint8>(vm);
        wisp::uint8 regIndex1 = ReadArgument<wisp::uint8>(vm);

        SimpleVmRegister& reg0 = TranslateRegisterNumber(regIndex0, vm);
        SimpleVmRegister& reg1 = TranslateRegisterNumber(regIndex1, vm);

        reg0.value += reg1.value;

        return wisp::VmError::OK;
    }

    wisp::VmError AndRegToRegNative(wisp::Vm* vm)
    {
        wisp::uint8 regIndex0 = ReadArgument<wisp::uint8>(vm);
        wisp::uint8 regIndex1 = ReadArgument<wisp::uint8>(vm);

        SimpleVmRegister& reg0 = TranslateRegisterNumber(regIndex0, vm);
        SimpleVmRegister& reg1 = TranslateRegisterNumber(regIndex1, vm);

        reg0.value &= reg1.value;

        return wisp::VmError::OK;
    }

    wisp::VmError AddNumToRegNative(wisp::Vm* vm)
    {
        wisp::uint8 regIndex = ReadArgument<wisp::uint8>(vm);
        wisp::uint8 constant = ReadArgument<wisp::uint8>(vm);

        SimpleVmRegister& reg = TranslateRegisterNumber(regIndex, vm);

        reg.value += constant;

        return wisp::VmError::OK;
    }

    wisp::VmError SubNumFromRegNative(wisp::Vm* vm)
    {
        wisp::uint8 regIndex = ReadArgument<wisp::uint8>(vm);
        wisp::uint8 constant = ReadArgument<wisp::uint8>(vm);

        SimpleVmRegister& reg = TranslateRegisterNumber(regIndex, vm);

        reg.value -= constant;

        return wisp::VmError::OK;
    }

    wisp::VmError PrintContextNative(wisp::Vm* vm)
    {
        // not implemented yet
		for (wisp::uint32 i = 0; i < 32; ++i)
		{
			char regname[8] = { 0 };
			sprintf_s(regname, "R%d", i);
			PrintRegister(regname, m_context->regGeneral[i]);
		}

		printf("PC = 0x%I64X\n", m_context->regPc.Get());

        return wisp::VmError::OK;
    }

	wisp::VmError PrintHelloWorldNative(wisp::Vm* vm)
	{
		printf("Hello, World!\n");

		return wisp::VmError::OK;
	}

	wisp::VmError JumpNative(wisp::Vm* vm)
	{
		wisp::uint8 dst = ReadArgument<wisp::uint8>(vm);

		// Calculate the PC relative destination... this would be more efficient without the sub...
		wisp::uint8 calcDest = (m_context->regPc.Get() - sizeof(wisp::uint8)) + dst;

		m_context->regPc.GoTo(calcDest);

		return wisp::VmError::OK;
	}

    wisp::VmError EndNative(wisp::Vm* vm)
    {
        return wisp::VmError::HaltExecution;
    }

private:
    template<typename T>
    T ReadArgument(wisp::Vm* vm)
    {
        T ret = *reinterpret_cast<T*>(vm->GetMemory()->GetPhysicalMemory() + vm->GetContext()->regPc.Get());
        vm->GetContext()->regPc.Advance(sizeof(T));
        return ret;
    }

    SimpleVmRegister& TranslateRegisterNumber(wisp::uint8 num, wisp::Vm* vm)
    {
        SimpleVmContext* context = dynamic_cast<SimpleVmContext*>(vm->GetContext());
        REQUIRE(context != nullptr);
        REQUIRE(num >= 0);
        REQUIRE(num <= 32);

        return context->regGeneral[num];
    }

    void PrintRegister(const char* name, SimpleVmRegister& reg)
    {
        printf("Register %s [value: 0x%x]\n", name, reg.value);
    }

    SimpleVmContext* m_context;
};

typedef std::vector<wisp::uint8> ProgramCode;

TEST_CASE("Simple VM")
{
    wisp::uint64 memorySize = 16000 * 1000; // 16MB
    wisp::MemoryModule ram(memorySize);
    SimpleVmContext context;
    SimpleVmISA isa(&context);
    wisp::Vm instance(&context, &ram, &isa);

    const wisp::uint8 reg0 = 0;
    const wisp::uint8 reg1 = 1;
    const wisp::uint8 reg2 = 2;
    const wisp::uint8 reg3 = 3;
    const wisp::uint8 reg4 = 4;

    wisp::uint8 programCode[] =
    {
        SimpleVmISA::InstructionId::PrintContext,					// 0000
        SimpleVmISA::InstructionId::Store, reg0, 1,					// 0001
        SimpleVmISA::InstructionId::Store, reg1, 10,				// 0004
        SimpleVmISA::InstructionId::Store, reg3, 0x06,				// 0007
        SimpleVmISA::InstructionId::Store, reg4, 0x1E,				// 0010
        SimpleVmISA::InstructionId::AndRegToReg, reg3, reg4,		// 0013
        SimpleVmISA::InstructionId::MoveRegToReg, reg2, reg1,		// 0016
        SimpleVmISA::InstructionId::AddRegToReg, reg0, reg1,		// 0019
        SimpleVmISA::InstructionId::PrintContext,					// 0022
		SimpleVmISA::InstructionId::Jump, 7,						// 0023
		0x90, 0x90, 0x90, 0x90, 0x90,								// 0025
		SimpleVmISA::InstructionId::PrintHelloWorld,				// 0030
        SimpleVmISA::InstructionId::End								// 0031
    };

	// memset(ram.GetPhysicalMemory(), 0, ram.GetPhysicalMemorySize());

    memcpy(ram.GetPhysicalMemory(), programCode, sizeof(programCode));

    wisp::VmError err = instance.Execute(0);

    REQUIRE(err == wisp::VmError::OK);

    REQUIRE(context.regGeneral[reg0].value == 11);
    REQUIRE(context.regGeneral[reg2].value == 10);
    REQUIRE(context.regGeneral[reg3].value == 0x06);
}