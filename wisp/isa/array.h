#pragma once

#include "isa.h"

namespace wisp
{
	class ArrayISAModule : public WispISAModule
	{
	public:
		vmcore::VmError Create(std::unordered_map<InstructionCodes, isa_fn>& functionList) override;
	};
}