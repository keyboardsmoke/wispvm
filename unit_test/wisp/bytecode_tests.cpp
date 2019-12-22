#include "doctest/doctest.h"
#include "shared/types.h"
#include "wisp/register.h"
#include "wisp/context.h"
#include "wisp/isa/isa.h"
#include "vm/vm.h"
#include "wisp/bytecode.h"
#include "wisp/mm.h"
#include <iostream>

using namespace wisp;

static void PrintEFlags(const WispExecutionFlags& flags)
{
    std::cout << "EFLAGS::CarryFlag = " << (int)flags.CarryFlag << std::endl;
    std::cout << "EFLAGS::ParityFlag = " << (int)flags.ParityFlag << std::endl;
    std::cout << "EFLAGS::ZeroFlag = " << (int)flags.ZeroFlag << std::endl;
    std::cout << "EFLAGS::SignFlag = " << (int)flags.SignFlag << std::endl;
    std::cout << "EFLAGS::OverflowFlag = " << (int)flags.OverflowFlag << std::endl;
}

TEST_CASE("Wisp Bytecode")
{
    SUBCASE("Register Move")
    {
        ByteCodeGenerator gen;
        gen.Mov(GeneralPurposeRegisters::R0, IntegerValue(static_cast<uint16>(1)));
        gen.Mov(GeneralPurposeRegisters::R1, GeneralPurposeRegisters::R0);
        gen.Halt();

        WispContext context;
        WispISA isa;
        wisp::MemoryModule mm(gen.GetData().data(), gen.GetData().size());
        vmcore::Vm vm(&context, &mm, &isa);

        auto err = vm.Execute(0);
        REQUIRE(err == vmcore::VmError::OK);

        REQUIRE(context.regGp[0].GetValue().GetType() == ValueType::Integer);
        REQUIRE(context.regGp[0].GetValue().Get<IntegerValue>().GetType() == IntegerValueType::UInt16);
        REQUIRE(context.regGp[1].GetValue().GetType() == ValueType::Integer);
        REQUIRE(context.regGp[1].GetValue().Get<IntegerValue>().GetType() == IntegerValueType::UInt16);

        REQUIRE(context.regGp[0].GetValue().Get<IntegerValue>().Get<uint16>() == 1);
        REQUIRE(context.regGp[1].GetValue().Get<IntegerValue>().Get<uint16>() == 1);
    }

    SUBCASE("Register Clear")
    {
        ByteCodeGenerator gen;
        gen.Mov(GeneralPurposeRegisters::R0, IntegerValue(static_cast<uint16>(1)));
        gen.Clear(GeneralPurposeRegisters::R0);
        gen.Halt();

        WispContext context;
        WispISA isa;
        wisp::MemoryModule mm(gen.GetData().data(), gen.GetData().size());
        vmcore::Vm vm(&context, &mm, &isa);

        auto err = vm.Execute(0);
        REQUIRE(err == vmcore::VmError::OK);

        REQUIRE(context.regGp[0].GetValue().GetType() == ValueType::None);
    }

    SUBCASE("Compare and flags")
    {
        // EFLAGS = (F_PF|F_ZF|F_IF)

        ByteCodeGenerator gen;

        gen.Mov(GeneralPurposeRegisters::R0, IntegerValue(static_cast<uint32>(1)));
        gen.Compare(GeneralPurposeRegisters::R0, IntegerValue(static_cast<uint32>(1)));
        gen.Halt();

        WispContext context;
        WispISA isa;
        wisp::MemoryModule mm(gen.GetData().data(), gen.GetData().size());
        vmcore::Vm vm(&context, &mm, &isa);

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

        ByteCodeGenerator gen;

        gen.Mov(GeneralPurposeRegisters::R0, IntegerValue(static_cast<uint32>(1)));
        gen.Compare(GeneralPurposeRegisters::R0, IntegerValue(static_cast<uint32>(2)));
        gen.Halt();

        WispContext context;
        WispISA isa;
        wisp::MemoryModule mm(gen.GetData().data(), gen.GetData().size());
        vmcore::Vm vm(&context, &mm, &isa);

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

        ByteCodeGenerator gen;

        gen.Mov(GeneralPurposeRegisters::R0, IntegerValue(static_cast<uint32>(0)));
        gen.Test(GeneralPurposeRegisters::R0, IntegerValue(static_cast<uint32>(1)));
        gen.Halt();

        WispContext context;
        WispISA isa;
        wisp::MemoryModule mm(gen.GetData().data(), gen.GetData().size());
        vmcore::Vm vm(&context, &mm, &isa);

        auto err = vm.Execute(0);
        REQUIRE(err == vmcore::VmError::OK);

        // PrintEFlags(context.eflags);

        REQUIRE(context.eflags.CarryFlag == 0);
        REQUIRE(context.eflags.ParityFlag == 1);
        REQUIRE(context.eflags.ZeroFlag == 1);
        REQUIRE(context.eflags.SignFlag == 0);
        REQUIRE(context.eflags.OverflowFlag == 0);
    }

    SUBCASE("Compare equality and conditional jump")
    {
        ByteCodeGenerator gen;
        gen.Mov(GeneralPurposeRegisters::R0, IntegerValue(static_cast<uint8>(0)));
        gen.Compare(GeneralPurposeRegisters::R0, IntegerValue(static_cast<uint8>(0)));
        gen.ConditionalJump(ConditionCode::Zero, IntegerValue(static_cast<uint8>(9))); // We have to calculate this manually for now...
        gen.Mov(GeneralPurposeRegisters::R1, IntegerValue(static_cast<uint8>(10)));
        gen.Halt();
        gen.Mov(GeneralPurposeRegisters::R1, IntegerValue(static_cast<uint8>(20)));
        gen.Halt();

        WispContext context;
        WispISA isa;
        wisp::MemoryModule mm(gen.GetData().data(), gen.GetData().size());
        vmcore::Vm vm(&context, &mm, &isa);

        auto err = vm.Execute(0);
        REQUIRE(err == vmcore::VmError::OK);

        REQUIRE(context.eflags.ZeroFlag == 1);
        REQUIRE(context.regGp[1].Get<IntegerValue>().Get<uint8>() == 20);
    }

    SUBCASE("Compare greater than and conditional jump")
    {
        ByteCodeGenerator gen;
        gen.Mov(GeneralPurposeRegisters::R0, IntegerValue(static_cast<uint8>(1)));
        gen.Compare(GeneralPurposeRegisters::R0, IntegerValue(static_cast<uint8>(0)));
        gen.ConditionalJump(ConditionCode::GreaterThan, IntegerValue(static_cast<uint8>(9))); // We have to calculate this manually for now...
        gen.Mov(GeneralPurposeRegisters::R1, IntegerValue(static_cast<uint8>(10)));
        gen.Halt();
        gen.Mov(GeneralPurposeRegisters::R1, IntegerValue(static_cast<uint8>(20)));
        gen.Halt();

        WispContext context;
        WispISA isa;
        wisp::MemoryModule mm(gen.GetData().data(), gen.GetData().size());
        vmcore::Vm vm(&context, &mm, &isa);

        auto err = vm.Execute(0);
        REQUIRE(err == vmcore::VmError::OK);

        REQUIRE(context.regGp[1].Get<IntegerValue>().Get<uint8>() == 20);
    }
}