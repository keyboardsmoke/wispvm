#include "shared/types.h"
#include "register.h"

using namespace wisp;

void Register::CopyValue(Register& rhs)
{
    this->DestroyValue();

    if (IntegerValue* iv = dynamic_cast<IntegerValue*>(rhs.GetValue()))
    {
        this->SetValue(new IntegerValue(iv->GetInt<uint64>()));
    }
    else if (PointerValue* pv = dynamic_cast<PointerValue*>(rhs.GetValue()))
    {
        this->SetValue(new PointerValue(pv->GetOffset()));
    }
    else
    {
        // Not implemented.... uhh
        printf("Warning: Register::CopyValue tried to copy from an unknown value type.\n");
    }
}

void Register::TakeValue(Register& rhs)
{
    CopyValue(rhs);

    rhs.DestroyValue();
}

void Register::TransferValue(Register& to)
{
    to.TakeValue(*this);
}

void Register::DestroyValue()
{
    if (m_value != nullptr)
        delete m_value;

    m_value = nullptr;
}