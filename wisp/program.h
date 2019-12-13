#pragma once

#include "shared/types.h"
#include <vector>

namespace wisp
{
    enum class VmError;
    class Vm;

	class Program
	{
		struct ProgramHeader
		{
			static const uint32 WISP_MAGIC = 0x77697370; // 'wisp'

			// Seed dictates how the following values are encoded
			// That way, when we use the seed to decode magic
			// We can still ensure integrity without having an obvious static magic
			uint64 seed;
			uint32 magic;
			uint32 ep;
			uint32 bytecodeCrc;
		};

		static std::vector<wisp::uint8> CreateProgram(std::vector<wisp::uint8>& byteCode);
		VmError ExecuteProgram(Vm* vm, void *program, uint32 size);
	};
}