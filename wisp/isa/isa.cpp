#include "shared/types.h"
#include "isa.h"
#include "vm/vm.h"

using namespace wisp;
using namespace vmcore;

/*
VmError Store(WispISA* isa, Vm* vm)
{
	return VmError::OK;
}

VmError Load(WispISA* isa, Vm* vm)
{
	return VmError::OK;
}

VmError Clear(WispISA* isa, Vm* vm)
{
	return VmError::OK;
}

VmError Move(WispISA* isa, Vm* vm)
{
	return VmError::OK;
}

VmError MoveConstant8(WispISA* isa, Vm* vm)
{
	return VmError::OK;
}

VmError MoveConstant16(WispISA* isa, Vm* vm)
{
	return VmError::OK;
}

VmError MoveConstant32(WispISA* isa, Vm* vm)
{
	return VmError::OK;
}

VmError MoveConstant64(WispISA* isa, Vm* vm)
{
	return VmError::OK;
}

VmError Jump(WispISA* isa, Vm* vm)
{
	return VmError::OK;
}

VmError Call(WispISA* isa, Vm* vm)
{
	return VmError::OK;
}

std::function<VmError(WispISA*, Vm*)> g_definitionTable[] =
{
	// General Purpose
	Store,
	StoreConstant,
	Load,
	Clear,
	Move,
	MoveConstant,
	Jump,
	ConditionalJump,
	Call,
	Return,
	Halt,

	// Comparison
	Eq,
	Lt,
	Le,
	Test,

	// Tables

	// Arrays

	// Math (Register to Register only)
	Add,
	Sub,
	Mul,
	Mod,
	Pow,
	Div,
	IDiv,
	BAnd,
	BOr,
	BXor,
	Shl,
	Shr,
	Unm,
	BNot,
	Not
};*/

VmError WispISA::ExecuteInstruction(Vm* vm)
{
	const uint16 id = ReadArgument<uint16>(vm);

	// return g_definitionTable[id].fn(this, vm);
	return VmError::OK;
}