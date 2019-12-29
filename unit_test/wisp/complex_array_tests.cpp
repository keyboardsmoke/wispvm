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

TEST_CASE("Complex Array Tests")
{
    SUBCASE("Initialize and compare array")
    {
        ByteCodeGenerator gen;

        uint32 programStart = gen.CreateIntegerArray(GeneralPurposeRegisters::R0, IntegerValueType::UInt8);
        gen.Mov(GeneralPurposeRegisters::R1, IntegerValue(static_cast<uint8>(0xfe)));
        gen.PushArray(GeneralPurposeRegisters::R0, GeneralPurposeRegisters::R1);
        gen.Halt();

        WispContext context;
        WispISA isa;
        wisp::MemoryModule mm(gen.GetData().data(), gen.GetData().size());
        vmcore::Vm vm(&context, &mm, &isa);

        auto err = vm.Execute(programStart);
        REQUIRE(err == vmcore::VmError::OK);
        REQUIRE(context.regGp[0].GetValue().GetType() == ValueType::Array);
        ArrayValue& arr = context.regGp[0].Get<ArrayValue>();
        REQUIRE(arr.IsEmpty() == false);
        REQUIRE(arr.GetValue(0).Get<IntegerValue>().Get<uint8>() == 0xfe);
    }
}