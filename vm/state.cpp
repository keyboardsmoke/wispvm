#include "shared/types.h"
#include "state.h"
#include "vm.h"

using namespace wisp;

VmError State::Release()
{
    for (uint32 i = 0; i < GpRegisterCount; ++i)
    {
        this->regGeneral[i].DestroyValue();
    }

    for (uint32 i = 0; i < ArgRegisterCount; ++i)
    {
        this->regArguments[i].DestroyValue();
    }

    this->regReturn.DestroyValue();
    this->regPc.DestroyValue();

    return VmError::OK;
}