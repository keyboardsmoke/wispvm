#include "shared/types.h"
#include "vm/vm.h"

using namespace vmcore;

#include "register.h"

using namespace wisp;

void RegisterFP::CopyValue(RegisterFP& rhs)
{
    this->m_value = rhs.m_value;
}

void RegisterFP::DestroyValue()
{
    m_value = FPValue();
}

void RegisterInt::CopyValue(RegisterInt& rhs)
{
    this->m_value = rhs.m_value;
}

void RegisterInt::DestroyValue()
{
    m_value = IntegerValue();
}