#pragma once

#include "isa.h"

namespace wisp
{
	class ControlFlowISAModule : public WispISAModule
	{
	public:
		vmcore::VmError Create(isa_fn* functionList) override;
	};
}