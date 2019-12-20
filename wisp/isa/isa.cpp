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

using namespace wisp;
using namespace vmcore;

VmError WispISA::Initialize()
{
    VmError err = VmError::OK;

    // CMP, TEST, etc.
    err = CompareISAModule().Create(m_isaFunctions); if (err != VmError::OK) { return err; }

    // JMP, COND JMP, CALL, RET, HALT
    err = ControlFlowISAModule().Create(m_isaFunctions); if (err != VmError::OK) { return err; }

    // LOAD, STORE
    err = LoadStoreISAModule().Create(m_isaFunctions); if (err != VmError::OK) { return err; }

    // MOV, MOVFP
    err = MoveISAModule().Create(m_isaFunctions); if (err != VmError::OK) { return err; }

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

    assert(id < (sizeof(m_isaFunctions) / sizeof(m_isaFunctions[0])));
    assert(m_isaFunctions[id] != nullptr);
    return m_isaFunctions[id](this, vm, context, startingPc);
}