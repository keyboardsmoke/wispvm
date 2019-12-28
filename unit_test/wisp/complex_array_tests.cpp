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

static uint32 WriteStringToGenerator(ByteCodeGenerator& gen, const std::string& str)
{
    auto pos = gen.GetData().size();

    for (size_t i = 0; i < str.length(); ++i)
    {
        gen.GetData().push_back(str.data()[i]);
    }

    gen.GetData().push_back(0);

    return static_cast<uint32>(pos);
}

TEST_CASE("Complex Array Tests")
{
    SUBCASE("Initialize and compare array")
    {
        ByteCodeGenerator gen;

        uint32 programStart = gen.Mov(GeneralPurposeRegisters::R0, IntegerValue(static_cast<uint32>(0)));
        gen.CreateString(GeneralPurposeRegisters::R1, GeneralPurposeRegisters::R0);
        gen.Halt();

        WispContext context;
        WispISA isa;
        wisp::MemoryModule mm(gen.GetData().data(), gen.GetData().size());
        vmcore::Vm vm(&context, &mm, &isa);

        auto err = vm.Execute(programStart);
        REQUIRE(err == vmcore::VmError::OK);
        
    }

    SUBCASE("Format String Print")
    {
        ByteCodeGenerator gen;

        uint32 formatString = WriteStringToGenerator(gen, "This is an integer {0}, this is a float {1}, yep.");

        uint32 programStart = gen.Mov(GeneralPurposeRegisters::R1, IntegerValue(static_cast<uint32>(0)));
        gen.CreateString(GeneralPurposeRegisters::R0, GeneralPurposeRegisters::R1);
        gen.Mov(GeneralPurposeRegisters::R1, IntegerValue(123));
        gen.Mov(GeneralPurposeRegisters::R2, FPValue(3.23f));
        gen.FormatString();
        gen.Halt();

        WispContext context;
        WispISA isa;
        wisp::MemoryModule mm(gen.GetData().data(), gen.GetData().size());
        vmcore::Vm vm(&context, &mm, &isa);

        auto err = vm.Execute(programStart);
        REQUIRE(err == vmcore::VmError::OK);
        REQUIRE(context.regGp[0].GetValue().IsStringValue());
        REQUIRE(context.regGp[0].GetValue().Get<StringValue>().GetString() == "This is an integer 123, this is a float 3.23, yep.");
    }
}