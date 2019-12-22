#pragma once

namespace wisp
{
	namespace encode
	{
		vmcore::VmError ReadValueWithEncoding(vmcore::Vm* vm, Value& value);
		vmcore::VmError ReadValueWithEncodingToRegister(vmcore::Vm* vm, Register& reg);

		template<typename T>
		T ReadArgument(vmcore::Vm* vm)
		{
			T ret = vm->GetMemory()->Read<T>(vm->GetContext()->regPc.Get());
			vm->GetContext()->regPc.Advance(sizeof(T));
			return ret;
		}
	}
}