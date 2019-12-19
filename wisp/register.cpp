#include "shared/types.h"
#include "vm/vm.h"

using namespace vmcore;

#include "register.h"

using namespace wisp;

void RegisterInt::CopyValue(RegisterInt& rhs)
{
    UNREFERENCED_PARAMETER(rhs);
}

void RegisterInt::DestroyValue()
{
    m_value = IntegerValue();
}