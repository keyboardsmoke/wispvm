#pragma once

#include "isa.h"

namespace wisp
{
	class CompareISAModule : public WispISAModule
	{
	public:
		vmcore::VmError Create(isa_fn* functionList) override;
	};
}