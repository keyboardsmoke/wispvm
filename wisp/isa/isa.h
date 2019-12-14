#pragma once

#include "vm/isa.h"

namespace wisp
{
    class WispISA : public vmcore::ISA
    {
    public:
		vmcore::VmError ExecuteInstruction(vmcore::Vm* vm) override;

		template<typename T>
		T ReadArgument(vmcore::Vm* vm)
		{
			T ret = *reinterpret_cast<T*>(vm->GetMemory()->GetPhysicalMemory() + vm->GetContext()->regPc.Get());
			vm->GetContext()->regPc.Advance(sizeof(T));
			return ret;
		}
    };
}