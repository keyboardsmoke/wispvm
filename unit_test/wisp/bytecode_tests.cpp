#include "doctest/doctest.h"
#include "shared/types.h"
#include "wisp/register.h"
#include "wisp/context.h"
#include "wisp/isa/isa.h"
#include "vm/vm.h"
#include "wisp/bytecode.h"
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
    SUBCASE("Compare and flags")
    {
        // EFLAGS = (F_PF|F_ZF|F_IF)

        wisp::ByteCodeGenerator gen;

        gen.Mov(wisp::GeneralPurposeRegisters::R0, wisp::IntegerValue(static_cast<uint32>(1)));
        gen.Compare(wisp::GeneralPurposeRegisters::R0, wisp::IntegerValue(static_cast<uint32>(1)));
        gen.Halt();

        wisp::WispContext context;
        wisp::WispISA isa;
        vmcore::MemoryModule mm(0x1000);
        vmcore::Vm vm(&context, &mm, &isa);

        memcpy(mm.GetPhysicalMemory(), gen.GetData().data(), gen.GetData().size());

        auto err = vm.Execute(0);
        REQUIRE(err == vmcore::VmError::OK);

        // PrintEFlags(context.eflags);

        REQUIRE(context.eflags.CarryFlag == 0);
        REQUIRE(context.eflags.ParityFlag == 1);
        REQUIRE(context.eflags.ZeroFlag == 1);
        REQUIRE(context.eflags.SignFlag == 0);
        REQUIRE(context.eflags.OverflowFlag == 0);
    }

    SUBCASE("Compare and flags negative result")
    {
        // EFLAGS = (F_CF|F_PF|F_AF|F_SF|F_IF)

        wisp::ByteCodeGenerator gen;

        gen.Mov(wisp::GeneralPurposeRegisters::R0, wisp::IntegerValue(static_cast<uint32>(1)));
        gen.Compare(wisp::GeneralPurposeRegisters::R0, wisp::IntegerValue(static_cast<uint32>(2)));
        gen.Halt();

        wisp::WispContext context;
        wisp::WispISA isa;
        vmcore::MemoryModule mm(0x1000);
        vmcore::Vm vm(&context, &mm, &isa);

        memcpy(mm.GetPhysicalMemory(), gen.GetData().data(), gen.GetData().size());

        auto err = vm.Execute(0);
        REQUIRE(err == vmcore::VmError::OK);

        // PrintEFlags(context.eflags);

        REQUIRE(context.eflags.CarryFlag == 1);
        REQUIRE(context.eflags.ParityFlag == 1);
        REQUIRE(context.eflags.ZeroFlag == 0);
        REQUIRE(context.eflags.SignFlag == 1);
        REQUIRE(context.eflags.OverflowFlag == 0);
    }

    SUBCASE("Test and flags")
    {
        // EFLAGS = (F_PF|F_ZF|F_IF)

        wisp::ByteCodeGenerator gen;

        gen.Mov(wisp::GeneralPurposeRegisters::R0, wisp::IntegerValue(static_cast<uint32>(0)));
        gen.Test(wisp::GeneralPurposeRegisters::R0, wisp::IntegerValue(static_cast<uint32>(1)));
        gen.Halt();

        wisp::WispContext context;
        wisp::WispISA isa;
        vmcore::MemoryModule mm(0x1000);
        vmcore::Vm vm(&context, &mm, &isa);

        memcpy(mm.GetPhysicalMemory(), gen.GetData().data(), gen.GetData().size());

        auto err = vm.Execute(0);
        REQUIRE(err == vmcore::VmError::OK);

        // PrintEFlags(context.eflags);

        REQUIRE(context.eflags.CarryFlag == 0);
        REQUIRE(context.eflags.ParityFlag == 1);
        REQUIRE(context.eflags.ZeroFlag == 1);
        REQUIRE(context.eflags.SignFlag == 0);
        REQUIRE(context.eflags.OverflowFlag == 0);
    }
}