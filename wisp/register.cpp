#include "shared/types.h"
#include "vm/vm.h"

using namespace vmcore;

#include "register.h"

using namespace wisp;

void Register::CopyValue(Register& rhs)
{
    this->m_value = rhs.m_value;
}

void Register::DestroyValue()
{
    m_value = Value();
}