#pragma once

#include "shared/types.h"

namespace vmcore
{
    enum class VmError;

	// Your implemented context will always include this basic register
	// You can implement other registers however you like, but this one is unique
	class ProgramCounter
	{
	public:
        ProgramCounter() : m_offset(0) {}

		void Advance(uint64 position)
		{
			m_offset += position;
		}

		void GoTo(uint64 offset)
		{
			m_offset = offset;
		}

        uint64 Get()
        {
            return m_offset;
        }

	private:
		uint64 m_offset;
	};

	class Context
	{
	public:
		virtual ~Context() {}

		// The program counter, this is universal across all VMs
		ProgramCounter regPc;
	};
}