#include "doctest/doctest.h"
#include "vm/vm.h"

// Reserved Register Numbers
const uint8 pc = 0xff;

class SimpleVmRegister
{
public:
    SimpleVmRegister() : value(0) {}

    uint8 value;
};

class SimpleVmContext : public vmcore::Context
{
public:
    SimpleVmRegister regGeneral[32];
};

class SimpleVmMemoryModule : public vmcore::MemoryModule
{
public:
    SimpleVmMemoryModule(uint8* data, uint64 size) : vmcore::MemoryModule(data, size) {}
    SimpleVmMemoryModule(uint64 totalMemorySize) : vmcore::MemoryModule(totalMemorySize) {}

    bool CanExecute(uint64 offset, uint64 size) override
    {
        return true;
    }

    bool CanRead(uint64 offset, uint64 size) override
    {
        return true;
    }

    bool CanWrite(uint64 offset, uint64 size) override
    {
        return true;
    }
};

class SimpleVmISA : public vmcore::ISA
{
public:
    SimpleVmISA(SimpleVmContext* context) : m_context(context) {}

    enum InstructionId : uint8
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

    vmcore::VmError Initialize() override
    {
        return vmcore::VmError::OK;
    }

    vmcore::VmError ExecuteInstruction(vmcore::Vm* vm) override
    {
        uint64 oldPc = vm->GetContext()->regPc.Get();
        uint8 id = vm->GetMemory()->Read<uint8>(oldPc);
        vm->GetContext()->regPc.Advance(sizeof(uint8));
        uint64 newPc = vm->GetContext()->regPc.Get();

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
                return vmcore::VmError::InvalidInstruction;
            }
        }
    }

    vmcore::VmError StoreNative(vmcore::Vm* vm)
    {
        uint8 regIndex = ReadArgument<uint8>(vm);
        uint8 constant = ReadArgument<uint8>(vm);

        SimpleVmRegister& reg = TranslateRegisterNumber(regIndex, vm);

        reg.value = constant;

        return vmcore::VmError::OK;
    }

    vmcore::VmError MoveRegToRegNative(vmcore::Vm* vm)
    {
        uint8 regIndex0 = ReadArgument<uint8>(vm);
        uint8 regIndex1 = ReadArgument<uint8>(vm);

        SimpleVmRegister& reg0 = TranslateRegisterNumber(regIndex0, vm);
        SimpleVmRegister& reg1 = TranslateRegisterNumber(regIndex1, vm);

        reg0.value = reg1.value;

        return vmcore::VmError::OK;
    }

    vmcore::VmError AddRegToRegNative(vmcore::Vm* vm)
    {
        uint8 regIndex0 = ReadArgument<uint8>(vm);
        uint8 regIndex1 = ReadArgument<uint8>(vm);

        SimpleVmRegister& reg0 = TranslateRegisterNumber(regIndex0, vm);
        SimpleVmRegister& reg1 = TranslateRegisterNumber(regIndex1, vm);

        reg0.value += reg1.value;

        return vmcore::VmError::OK;
    }

    vmcore::VmError AndRegToRegNative(vmcore::Vm* vm)
    {
        uint8 regIndex0 = ReadArgument<uint8>(vm);
        uint8 regIndex1 = ReadArgument<uint8>(vm);

        SimpleVmRegister& reg0 = TranslateRegisterNumber(regIndex0, vm);
        SimpleVmRegister& reg1 = TranslateRegisterNumber(regIndex1, vm);

        reg0.value &= reg1.value;

        return vmcore::VmError::OK;
    }

    vmcore::VmError AddNumToRegNative(vmcore::Vm* vm)
    {
        uint8 regIndex = ReadArgument<uint8>(vm);
        uint8 constant = ReadArgument<uint8>(vm);

        SimpleVmRegister& reg = TranslateRegisterNumber(regIndex, vm);

        reg.value += constant;

        return vmcore::VmError::OK;
    }

    vmcore::VmError SubNumFromRegNative(vmcore::Vm* vm)
    {
        uint8 regIndex = ReadArgument<uint8>(vm);
        uint8 constant = ReadArgument<uint8>(vm);

        SimpleVmRegister& reg = TranslateRegisterNumber(regIndex, vm);

        reg.value -= constant;

        return vmcore::VmError::OK;
    }

    vmcore::VmError PrintContextNative(vmcore::Vm* vm)
    {
        // not implemented yet
        for (uint32 i = 0; i < 32; ++i)
        {
            char regname[8] = { 0 };
            sprintf_s(regname, "R%d", i);
            PrintRegister(regname, m_context->regGeneral[i]);
        }

        printf("PC = 0x%I64X\n", m_context->regPc.Get());

        return vmcore::VmError::OK;
    }

    vmcore::VmError PrintHelloWorldNative(vmcore::Vm* vm)
    {
        printf("Hello, World!\n");

        return vmcore::VmError::OK;
    }

    vmcore::VmError JumpNative(vmcore::Vm* vm)
    {
        uint64 pc = m_context->regPc.Get();
        uint8 dst = ReadArgument<uint8>(vm);

        // Calculate the PC relative destination... this would be more efficient without the sub...
        uint8 calcDest = (static_cast<uint8>(pc) - sizeof(uint8)) + dst;

        m_context->regPc.GoTo(calcDest);

        return vmcore::VmError::OK;
    }

    vmcore::VmError EndNative(vmcore::Vm* vm)
    {
        return vmcore::VmError::HaltExecution;
    }

private:
    template<typename T>
    T ReadArgument(vmcore::Vm* vm)
    {
        T ret = vm->GetMemory()->Read<T>(vm->GetContext()->regPc.Get());
        vm->GetContext()->regPc.Advance(sizeof(T));
        return ret;
    }

    SimpleVmRegister& TranslateRegisterNumber(uint8 num, vmcore::Vm* vm)
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

typedef std::vector<uint8> ProgramCode;

TEST_CASE("Simple VM")
{
    const uint8 reg0 = 0;
    const uint8 reg1 = 1;
    const uint8 reg2 = 2;
    const uint8 reg3 = 3;
    const uint8 reg4 = 4;
    const uint8 reg5 = 5;

    uint8 programCode[] =
    {
        SimpleVmISA::InstructionId::Store, reg0, 1,
        SimpleVmISA::InstructionId::Store, reg1, 10,
        SimpleVmISA::InstructionId::Store, reg3, 0x06,
        SimpleVmISA::InstructionId::Store, reg4, 0x1E,
        SimpleVmISA::InstructionId::AndRegToReg, reg3, reg4,
        SimpleVmISA::InstructionId::MoveRegToReg, reg2, reg1,
        SimpleVmISA::InstructionId::AddRegToReg, reg0, reg1,
        SimpleVmISA::InstructionId::Jump, 7,
        0x90, 0x90, 0x90, 0x90, 0x90,
        SimpleVmISA::InstructionId::Store, reg5, 0xff,
        SimpleVmISA::InstructionId::End
    };


    uint64 memorySize = 16777216; // 16MB
    SimpleVmMemoryModule ram(programCode, sizeof(programCode));
    SimpleVmContext context;
    SimpleVmISA isa(&context);
    vmcore::Vm instance(&context, &ram, &isa);

    vmcore::VmError err = instance.Execute(0);

    REQUIRE(err == vmcore::VmError::OK);

    REQUIRE(context.regGeneral[reg0].value == 11);
    REQUIRE(context.regGeneral[reg2].value == 10);
    REQUIRE(context.regGeneral[reg3].value == 0x06);
    REQUIRE(context.regGeneral[reg5].value == 0xff);
}