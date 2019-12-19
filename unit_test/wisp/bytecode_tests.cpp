#include "doctest/doctest.h"
#include "shared/types.h"
#include "wisp/register.h"
#include "wisp/isa/isa.h"

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

            // 
             static_cast<uint8>(wisp::InstructionCodes::CompareConstant),
        };
    }
}