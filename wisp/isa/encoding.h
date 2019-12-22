#pragma once

namespace wisp
{
	namespace encode
	{
		vmcore::VmError SetIntegerRegisterValueWithEncoding(vmcore::Vm* vm, Register& reg, IntegerValueType encoding, uint64 base = 0u);
		vmcore::VmError GetIntegerConstantValueWithEncoding(vmcore::Vm* vm, IntegerValue& value, IntegerValueType encoding);
		vmcore::VmError SetFPRegisterValueWithEncoding(vmcore::Vm* vm, Register& reg, FPValueType encoding);
		vmcore::VmError GetRelativeAddressDestinationFromPc(vmcore::Vm* vm, WispContext* context, uint8 encoding, uint64 pc, uint64* addr);

		template<typename T>
		T ReadArgument(vmcore::Vm* vm)
		{
			T ret = vm->GetMemory()->Read<T>(vm->GetContext()->regPc.Get());
			vm->GetContext()->regPc.Advance(sizeof(T));
			return ret;
		}
	}
}