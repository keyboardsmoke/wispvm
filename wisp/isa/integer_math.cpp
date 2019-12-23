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

vmcore::VmError MathISAModule::Create(std::unordered_map<InstructionCodes, isa_fn>& functionList)
{
    UNREFERENCED_PARAMETER(functionList);

    /*
    functionList[InstructionCodes::Add] = Add;
    functionList[InstructionCodes::Sub] = Sub;
    functionList[InstructionCodes::Mul] = Mul;
    functionList[InstructionCodes::Mod] = Mod;
    functionList[InstructionCodes::Pow] = Pow;
    functionList[InstructionCodes::Div] = Div;
    functionList[InstructionCodes::IDiv] = IDiv;
    functionList[InstructionCodes::BAnd] = BAnd;
    functionList[InstructionCodes::BOr] = BOr;
    functionList[InstructionCodes::BXor] = BXor;
    functionList[InstructionCodes::Shl] = Shl;
    functionList[InstructionCodes::Shr] = Shr;
    functionList[InstructionCodes::Unm] = Unm;
    functionList[InstructionCodes::BNot] = BNot;
    functionList[InstructionCodes::Not] = Not;
    */

    return vmcore::VmError::OK;
}