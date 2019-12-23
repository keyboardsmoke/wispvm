#include "shared/types.h"
#include "isa.h"
#include "vm/vm.h"
#include "../context.h"
#include "isa/encoding.h"
#include "isa/compare.h"
#include "isa/control_flow.h"
//#include "isa/integer_math.h"
#include "isa/load_store.h"
#include "isa/move.h"
#include "isa/string.h"
#include "isa/syscall.h"

using namespace wisp;
using namespace vmcore;

VmError WispISA::Initialize()
{
    // Register the base handlers for the VM
    VmError err = VmError::OK;

    // CMP, TEST, etc.
    err = RegisterModule(new CompareISAModule()); if (err != VmError::OK) { return err; }

    // JMP, COND JMP, CALL, RET, HALT
    RegisterModule(new ControlFlowISAModule()); if (err != VmError::OK) { return err; }

    // LOAD, STORE
    RegisterModule(new LoadStoreISAModule()); if (err != VmError::OK) { return err; }

    // MOV, MOVFP
    RegisterModule(new MoveISAModule()); if (err != VmError::OK) { return err; }

    // Complex STRING
    RegisterModule(new StringISAModule()); if (err != VmError::OK) { return err; }

    // SYSCALL
    RegisterModule(new SyscallISAModule()); if (err != VmError::OK) { return err; }

    return err;
}

VmError WispISA::ExecuteInstruction(Vm* vm)
{
    WispContext* context = dynamic_cast<WispContext*>(vm->GetContext());
    if (context == nullptr)
    {
        return VmError::InvalidContext;
    }

    uint64 startingPc = context->regPc.Get();

    const uint8 id = encode::ReadArgument<uint8>(vm);

    auto f = m_isaFunctions.find(static_cast<InstructionCodes>(id));
    if (f != m_isaFunctions.end())
    {
        return f->second.func(this, f->second.mod, vm, context, startingPc);
    }

    return VmError::InvalidInstruction;
}