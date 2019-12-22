#include "shared/types.h"
#include "vm/vm.h"
#include "fp_value.h"
#include "integer_value.h"
#include "register.h"
#include "context.h"
#include "compare.h"
#include "encoding.h"

using namespace wisp;
using namespace vmcore;

VmError SetFlagsForIntegerOperation(WispContext* context, const WispExecutionFlags& dirty, IntegerValue& dst, IntegerValue& src1, IntegerValue& src2)
{
	static uint32 parity_table[8] =
	{
		0x96696996,
		0x69969669,
		0x69969669,
		0x96696996,
		0x69969669,
		0x96696996,
		0x96696996,
		0x69969669,
	};

	// We'll handle the SF/ZF/PF first.
	if (dirty.SignFlag)
	{
		std::visit([context](auto&& destination)
			{
				typedef decltype(destination) dtype;
				dtype signMaskShift = static_cast<dtype>((sizeof(dtype) * 8) - 1);
				dtype signMask = (static_cast<dtype>(1) << signMaskShift);
				context->eflags.SignFlag = ((destination & signMask) == signMask);
			}, dst.GetValue());
	}

	if (dirty.ZeroFlag)
	{
		std::visit([context](auto&& destination)
			{
				typedef decltype(destination) dtype;
				dtype neg = static_cast<dtype>(~0);
				context->eflags.ZeroFlag = ((destination & neg) == 0);
			}, dst.GetValue());
	}

	if (dirty.ParityFlag)
	{
		std::visit([context](auto&& destination)
			{
				auto piece = destination & 0xff;

				context->eflags.ParityFlag = (((parity_table[piece / 32] >> (piece % 32)) & 1) == 0);
			}, dst.GetValue());
	}

	IntegerValue borrowChain = (dst & (~src1 | src2)) | (~src1 & src2);

	// CF/OF happens here
	if (dirty.CarryFlag)
	{
		std::visit([context](auto&& destination)
			{
				typedef decltype(destination) dtype;
				dtype signMaskShift = static_cast<dtype>((sizeof(dtype) * 8) - 1);
				dtype signMask = (static_cast<dtype>(1) << signMaskShift);
				context->eflags.CarryFlag = ((destination & signMask) == signMask);
			}, borrowChain.GetValue());
	}

	if (dirty.OverflowFlag)
	{
		std::visit([context](auto&& destination)
			{
				typedef decltype(destination) dtype;
				dtype overflowMaskShift = static_cast<dtype>((sizeof(dtype) * 8) - 2);
				dtype xorMask = (destination >> overflowMaskShift);
				context->eflags.OverflowFlag = (((xorMask) ^ ((xorMask) >> 1)) & 1);
			}, borrowChain.GetValue());
	}

	return VmError::OK;
}

VmError SetFlagsForIntegerOperation(WispContext* context, IntegerValue& dst, IntegerValue& src1, IntegerValue& src2)
{
	WispExecutionFlags ef;
	ef.CarryFlag = 1u;
	ef.ParityFlag = 1u;
	ef.ZeroFlag = 1u;
	ef.SignFlag = 1u;
	ef.OverflowFlag = 1u;
	return SetFlagsForIntegerOperation(context, ef, dst, src1, src2);
}

static VmError Compare(WispISA* isa, Vm* vm, WispContext* context, uint64 instructionPc)
{
	UNREFERENCED_PARAMETER(isa);
	UNREFERENCED_PARAMETER(instructionPc);

	Register r1 = context->regGp[encode::ReadArgument<uint8>(vm)];
	Register r2 = context->regGp[encode::ReadArgument<uint8>(vm)];

	IntegerValue i1 = r1.GetValue().Get<IntegerValue>();
	IntegerValue i2 = r2.GetValue().Get<IntegerValue>();

	IntegerValue result = i1 - i2;

	// the result could have "any" type in the variant type...
	SetFlagsForIntegerOperation(context, result, i1, i2);

	return VmError::OK;
}

static VmError CompareConstant(WispISA* isa, Vm* vm, WispContext* context, uint64 instructionPc)
{
	UNREFERENCED_PARAMETER(isa);
	UNREFERENCED_PARAMETER(instructionPc);

	Register r1 = context->regGp[encode::ReadArgument<uint8>(vm)];

	Value c1;
	VmError err = encode::ReadValueWithEncoding(vm, c1);
	if (err != VmError::OK)
		return err;

	assert(c1.GetType() == ValueType::Integer);

	IntegerValue i1 = r1.GetValue().Get<IntegerValue>();
	IntegerValue i2 = c1.Get<IntegerValue>();

	IntegerValue result = i1 - i2.GetValue();

	SetFlagsForIntegerOperation(context, result, i1, i2);

	return VmError::OK;
}

static VmError Test(WispISA* isa, Vm* vm, WispContext* context, uint64 instructionPc)
{
	UNREFERENCED_PARAMETER(isa);
	UNREFERENCED_PARAMETER(instructionPc);

	Register r1 = context->regGp[encode::ReadArgument<uint8>(vm)];
	Register r2 = context->regGp[encode::ReadArgument<uint8>(vm)];

	IntegerValue i1 = r1.GetValue().Get<IntegerValue>();
	IntegerValue i2 = r2.GetValue().Get<IntegerValue>();

	IntegerValue result = i1 & i2;

	std::visit([context](auto&& arg)
		{
			context->eflags.OverflowFlag = 0;
			context->eflags.ZeroFlag = (arg == 0);
			context->eflags.CarryFlag = 0;
		}, result.GetValue());

	WispExecutionFlags dirtyFlags;

	// These are calculated as usual
	dirtyFlags.ParityFlag = 1;
	dirtyFlags.SignFlag = 1;

	SetFlagsForIntegerOperation(context, result, i1, i2);

	return VmError::OK;
}

static VmError TestConstant(WispISA* isa, Vm* vm, WispContext* context, uint64 instructionPc)
{
	UNREFERENCED_PARAMETER(isa);
	UNREFERENCED_PARAMETER(instructionPc);

	Register r1 = context->regGp[encode::ReadArgument<uint8>(vm)];

	Value c1;
	VmError err = encode::ReadValueWithEncoding(vm, c1);
	if (err != VmError::OK)
		return err;

	assert(c1.GetType() == ValueType::Integer);

	IntegerValue i1 = r1.GetValue().Get<IntegerValue>();
	IntegerValue i2 = c1.Get<IntegerValue>();

	IntegerValue result = i1 & i2;

	std::visit([context](auto&& arg)
		{
			context->eflags.OverflowFlag = 0;
			context->eflags.ZeroFlag = (arg == 0);
			context->eflags.CarryFlag = 0;
		}, result.GetValue());

	WispExecutionFlags dirtyFlags;

	// These are calculated as usual
	dirtyFlags.ParityFlag = 1;
	dirtyFlags.SignFlag = 1;

	SetFlagsForIntegerOperation(context, result, i1, i2);

	return VmError::OK;
}

vmcore::VmError CompareISAModule::Create(isa_fn* functionList)
{
	functionList[static_cast<uint32>(InstructionCodes::Compare)] = Compare;
	functionList[static_cast<uint32>(InstructionCodes::CompareConstant)] = CompareConstant;
	functionList[static_cast<uint32>(InstructionCodes::Test)] = Test;
	functionList[static_cast<uint32>(InstructionCodes::TestConstant)] = TestConstant;

	return vmcore::VmError::OK;
}