#pragma once

namespace wisp {

typedef VmError(*RegisterMathOperation)(Register& lhs, Register& rhs);

#define CreateIntegerMathOperationTableRow(lhsValueType, lhsCppType, op) \
	{ \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, /* None */ \
		[](Register & lhs, Register & rhs) -> VmError { lhs.SetInteger<lhsCppType>(lhs.GetInteger<lhsCppType>() op rhs.GetInteger<int8>(), lhsValueType); return VmError::OK; }, /* Int8 */ \
		[](Register & lhs, Register & rhs) -> VmError { lhs.SetInteger<lhsCppType>(lhs.GetInteger<lhsCppType>() op rhs.GetInteger<int16>(), lhsValueType); return VmError::OK; }, /* Int16 */ \
		[](Register & lhs, Register & rhs) -> VmError { lhs.SetInteger<lhsCppType>(lhs.GetInteger<lhsCppType>() op rhs.GetInteger<int32>(), lhsValueType); return VmError::OK; }, /* Int32 */ \
		[](Register & lhs, Register & rhs) -> VmError { lhs.SetInteger<lhsCppType>(lhs.GetInteger<lhsCppType>() op rhs.GetInteger<int64>(), lhsValueType); return VmError::OK; }, /* Int64 */ \
		[](Register & lhs, Register & rhs) -> VmError { lhs.SetInteger<lhsCppType>(lhs.GetInteger<lhsCppType>() op rhs.GetInteger<uint8>(), lhsValueType); return VmError::OK; }, /* UInt8 */ \
		[](Register & lhs, Register & rhs) -> VmError { lhs.SetInteger<lhsCppType>(lhs.GetInteger<lhsCppType>() op rhs.GetInteger<uint16>(), lhsValueType); return VmError::OK; }, /* UInt16 */ \
		[](Register & lhs, Register & rhs) -> VmError { lhs.SetInteger<lhsCppType>(lhs.GetInteger<lhsCppType>() op rhs.GetInteger<uint32>(), lhsValueType); return VmError::OK; }, /* UInt32 */ \
		[](Register & lhs, Register & rhs) -> VmError { lhs.SetInteger<lhsCppType>(lhs.GetInteger<lhsCppType>() op rhs.GetInteger<uint64>(), lhsValueType); return VmError::OK; }, /* UInt64 */ \
		[](Register & lhs, Register & rhs) -> VmError { lhs.SetInteger<lhsCppType>(lhs.GetInteger<lhsCppType>() op static_cast<uint8>(rhs.GetBool()), lhsValueType); return VmError::OK; }, /* Bool */ \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, /* Float */ \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, /* Double */ \
		[](Register & lhs, Register & rhs) -> VmError { lhs.SetInteger<lhsCppType>(lhs.GetInteger<lhsCppType>() op rhs.GetInteger<uint64>(), lhsValueType); return VmError::OK; }, /* Pointer */ \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, /* NativeFunction */ \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, /* Table */ \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, /* Array */ \
	},

#define CreateFPMathOperationTableRow(lhsValueType, lhsCppType, op) \
	{ \
		/* None */ \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, /* None */ \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, /* Int8 */ \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, /* Int16 */ \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, /* Int32 */ \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, /* Int64 */ \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, /* UInt8 */ \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, /* UInt16 */ \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, /* UInt32 */ \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, /* UInt64 */ \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, /* Bool */ \
		[](Register & lhs, Register & rhs) -> VmError { lhs.SetFP<lhsCppType>(lhs.GetFP<lhsCppType>() op rhs.GetFP<float>(), lhsValueType); return VmError::OK; }, /* Float */ \
		[](Register & lhs, Register & rhs) -> VmError { lhs.SetFP<lhsCppType>(lhs.GetFP<lhsCppType>() op rhs.GetFP<double>(), lhsValueType); return VmError::OK; }, /* Double */ \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, /* Pointer */ \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, /* NativeFunction */ \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, /* Table */ \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, /* Array */ \
	},

#define CreateMathOperationTable(name, op) RegisterMathOperation name[static_cast<uint8>(ValueType::Max)][static_cast<uint8>(ValueType::Max)] = \
{ \
	{ \
		/* None */ \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
	}, \
	CreateIntegerMathOperationTableRow(ValueType::Int8, int8, op) \
	CreateIntegerMathOperationTableRow(ValueType::Int16, int16, op) \
	CreateIntegerMathOperationTableRow(ValueType::Int32, int32, op) \
	CreateIntegerMathOperationTableRow(ValueType::Int64, int64, op) \
	CreateIntegerMathOperationTableRow(ValueType::UInt8, uint8, op) \
	CreateIntegerMathOperationTableRow(ValueType::UInt16, uint16, op) \
	CreateIntegerMathOperationTableRow(ValueType::UInt32, uint32, op) \
	CreateIntegerMathOperationTableRow(ValueType::UInt64, uint64, op) \
	{ \
		/* Bool */ \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
	}, \
	CreateFPMathOperationTableRow(ValueType::Float, float, op) \
	CreateFPMathOperationTableRow(ValueType::Double, double, op) \
	{ \
		/* Pointer */ \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
	}, \
	{ \
		/* NativeFunction */ \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
	}, \
	{ \
		/* Table */ \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
	}, \
	{ \
		/* Array */ \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
		[](Register & lhs, Register & rhs) -> VmError { return VmError::RegisterMismatch; }, \
	}, \
};


CreateMathOperationTable(AddRegisterOperationTable, +);
CreateMathOperationTable(SubRegisterOperationTable, -);
CreateMathOperationTable(MulRegisterOperationTable, *);
CreateMathOperationTable(DivRegisterOperationTable, /);

// Bitwise will have to wait, I need to make a macro thingy that omits floating point
// CreateMathOperationTable(XorRegisterOperationTable, ^);
}