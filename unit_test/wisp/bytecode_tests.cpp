#include "doctest/doctest.h"
#include "shared/types.h"
#include "wisp/register.h"
#include "wisp/context.h"
#include "wisp/isa/isa.h"
#include "vm/vm.h"
#include <iostream>

static void PrintEFlags(const wisp::WispExecutionFlags& flags)
{
    std::cout << "EFLAGS::CarryFlag = " << (int)flags.CarryFlag << std::endl;
    std::cout << "EFLAGS::ParityFlag = " << (int)flags.ParityFlag << std::endl;
    std::cout << "EFLAGS::ZeroFlag = " << (int)flags.ZeroFlag << std::endl;
    std::cout << "EFLAGS::SignFlag = " << (int)flags.SignFlag << std::endl;
    std::cout << "EFLAGS::OverflowFlag = " << (int)flags.OverflowFlag << std::endl;
}

TEST_CASE("Wisp Bytecode")
{
    SUBCASE("Compare and Jump")
    {
        uint8 bytecode[] =
        {
            // MOV R0, 1
            static_cast<uint8>(wisp::InstructionCodes::MoveConstantInteger),
            0,
            static_cast<uint8>(wisp::IntegerValueType::UInt8),
            1,

            // CMP R0, 2
            static_cast<uint8>(wisp::InstructionCodes::CompareConstant),
            0,
            static_cast<uint8>(wisp::IntegerValueType::UInt8),
            2,

            // die
            static_cast<uint8>(wisp::InstructionCodes::Halt)
        };

        wisp::WispContext context;
        wisp::WispISA isa;
        vmcore::MemoryModule mm(0x1000);
        vmcore::Vm vm(&context, &mm, &isa);

        memcpy(mm.GetPhysicalMemory(), bytecode, sizeof(bytecode));

        auto err = vm.Execute(0);
        REQUIRE(err == vmcore::VmError::OK);

        PrintEFlags(context.eflags);

        REQUIRE(context.eflags.ZeroFlag == 0);
    }
}