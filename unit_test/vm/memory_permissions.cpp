#include "doctest/doctest.h"
#include "vm/vm.h"

using namespace vmcore;

class NullVmMemoryModule : public vmcore::MemoryModule
{
public:
	struct PageTableEntry
	{
		PageTableEntry() :
			Read(1u), Write(1u), Execute(1u) {}

		PageTableEntry(uint8 Readable, uint8 Writable, uint8 Executable) :
			Read(Readable), Write(Writable), Execute(Executable) {}

		uint8 Read : 1;
		uint8 Write : 1;
		uint8 Execute : 1;
	};

	NullVmMemoryModule(uint8* data, uint64 size) : vmcore::MemoryModule(data, size) 
	{
		m_pageTable = new PageTableEntry[(size / PageSize)]();
	}

	NullVmMemoryModule(uint64 totalMemorySize) : vmcore::MemoryModule(totalMemorySize) 
	{
		m_pageTable = new PageTableEntry[(totalMemorySize / PageSize)]();
	}

	~NullVmMemoryModule()
	{
		delete[] m_pageTable;
	}

	PageTableEntry* GetPTE(uint64 offset)
	{
		uint64 alignedOffset = PageAlign(offset);
		assert(alignedOffset < m_size);
		return &m_pageTable[alignedOffset / PageSize];
	}

	void Protect(uint64 offset, uint64 size, uint8 Readable, uint8 Writable, uint8 Executable)
	{
		uint64 alignedOffset = PageAlign(offset);
		uint64 alignedOffsetEnd = PageAlign(offset + size);

		assert(alignedOffset < m_size);
		assert(alignedOffsetEnd <= m_size);

		for (uint64 i = alignedOffset; i < alignedOffsetEnd; ++i)
		{
			PageTableEntry& pte = m_pageTable[alignedOffset / PageSize];

			pte.Read = Readable;
			pte.Write = Writable;
			pte.Execute = Executable;
		}
	}

	bool CanExecute(uint64 offset, uint64 size) override
	{
		PageTableEntry* p1 = GetPTE(offset);
		PageTableEntry* p2 = GetPTE(offset + size);

		assert(p1 && p2);

		return p1->Execute == 1u && p2->Execute == 1u;
	}

	bool CanRead(uint64 offset, uint64 size) override
	{
		PageTableEntry* p1 = GetPTE(offset);
		PageTableEntry* p2 = GetPTE(offset + size);

		assert(p1 && p2);

		return p1->Read == 1u && p2->Read == 1u;
	}

	bool CanWrite(uint64 offset, uint64 size) override
	{
		PageTableEntry* p1 = GetPTE(offset);
		PageTableEntry* p2 = GetPTE(offset + size);

		assert(p1 && p2);

		return p1->Write == 1u && p2->Write == 1u;
	}

private:
	PageTableEntry* m_pageTable;
};

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
		uint8 id = vm->GetMemory()->Read<uint8>(oldPc);
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
		NullVmMemoryModule ram(MemoryModule::PageSize * 2);
		Context context;
		NullVmISA isa;
		Vm instance(&context, &ram, &isa);

		// Not marked executable.....
		uint8 instruction = 0x90;

		ram.Write<uint8>(0, instruction);
		ram.Write<uint8>(MemoryModule::PageSize, instruction);
		ram.Protect(MemoryModule::PageSize, MemoryModule::PageSize, 1u, 1u, 0u);

		REQUIRE(instance.Execute(0) == VmError::OK);
		REQUIRE(instance.Execute(MemoryModule::PageSize) == VmError::AccessViolation);
	}
}