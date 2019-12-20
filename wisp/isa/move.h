#pragma once

#include "isa.h"

namespace wisp
{
	class MoveISAModule : public WispISAModule
	{
	public:
		vmcore::VmError Create(isa_fn* functionList) override;
	};
}