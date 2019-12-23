#pragma once

#include "isa.h"

namespace wisp
{
	enum class SystemCallIndices
	{
		Exit,					// explicit exit program system call, same as halt instruction
		Print,					// Print a format string with an arbitrary number of arguments
		Count,
	};

	class SyscallISAModule : public WispISAModule
	{
	public:
		vmcore::VmError Create(std::unordered_map<InstructionCodes, isa_fn>& functionList) override;

		std::unordered_map<SystemCallIndices, isa_fn>& GetSystemCallProcedures()
		{
			return m_syscall;
		}

	private:
		// TODO: We want to make this an overridable system like ISA modules, but not now. Lazy.
		std::unordered_map<SystemCallIndices, isa_fn> m_syscall;
	};
}