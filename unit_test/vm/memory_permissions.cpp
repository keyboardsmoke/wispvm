#include "doctest/doctest.h"
#include "vm/vm.h"

using namespace vmcore;

class NullVmISA : public vmcore::ISA
{
public:
	NullVmISA() {}

	vmcore::VmError Initialize() override
	{
		return vmcore::VmError::OK;
	}

	vmcore::VmError ExecuteInstruction(Vm* vm) override
	{
		uint64 oldPc = vm->GetContext()->regPc.Get();

		uint8 id = vm->GetMemory()->ReadWithPermissions<uint8>(vm->GetMemory()->GetPhysicalMemory() + oldPc);
		vm->GetContext()->regPc.Advance(sizeof(uint8));
		uint64 newPc = vm->GetContext()->regPc.Get();

		if (id == 0x90)
		{
			return VmError::HaltExecution;
		}

		return VmError::InvalidInstruction;
	}
};

TEST_CASE("VM Memory Permissions")
{
	SUBCASE("Execution")
	{
		MemoryModule ram(MemoryModule::PageSize * 2);
		Context context;
		NullVmISA isa;
		Vm instance(&context, &ram, &isa);

		// Not marked executable.....
		uint8 instruction = 0x90;

		PageTableEntry entry(1u, 1u, 1u);
		ram.WriteMemoryWithPagePermissions(ram.GetPhysicalMemory(), &instruction, 1u, entry);
		entry.Executable = 0u;
		ram.WriteMemoryWithPagePermissions(ram.GetPhysicalMemory() + MemoryModule::PageSize, &instruction, 1u, entry);

		REQUIRE(instance.Execute(0) == VmError::OK);
		REQUIRE(instance.Execute(MemoryModule::PageSize) == VmError::AccessViolation);
	}
}