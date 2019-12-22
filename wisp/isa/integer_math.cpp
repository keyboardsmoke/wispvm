#include "shared/types.h"
#include "vm/vm.h"
#include "fp_value.h"
#include "integer_value.h"
#include "register.h"
#include "context.h"
#include "integer_math.h"
#include "encoding.h"

using namespace wisp;
using namespace vmcore;

vmcore::VmError MathISAModule::Create(isa_fn* functionList)
{
    UNREFERENCED_PARAMETER(functionList);

    /*
    functionList[static_cast<uint32>(InstructionCodes::Add)] = Add;
    functionList[static_cast<uint32>(InstructionCodes::Sub)] = Sub;
    functionList[static_cast<uint32>(InstructionCodes::Mul)] = Mul;
    functionList[static_cast<uint32>(InstructionCodes::Mod)] = Mod;
    functionList[static_cast<uint32>(InstructionCodes::Pow)] = Pow;
    functionList[static_cast<uint32>(InstructionCodes::Div)] = Div;
    functionList[static_cast<uint32>(InstructionCodes::IDiv)] = IDiv;
    functionList[static_cast<uint32>(InstructionCodes::BAnd)] = BAnd;
    functionList[static_cast<uint32>(InstructionCodes::BOr)] = BOr;
    functionList[static_cast<uint32>(InstructionCodes::BXor)] = BXor;
    functionList[static_cast<uint32>(InstructionCodes::Shl)] = Shl;
    functionList[static_cast<uint32>(InstructionCodes::Shr)] = Shr;
    functionList[static_cast<uint32>(InstructionCodes::Unm)] = Unm;
    functionList[static_cast<uint32>(InstructionCodes::BNot)] = BNot;
    functionList[static_cast<uint32>(InstructionCodes::Not)] = Not;
    */

    return vmcore::VmError::OK;
}