#pragma once

#include "shared/types.h"

namespace wisp {

    typedef VmError(*RegisterMathOperation)(Register& lhs, Register& rhs);

#define UNHANDLED_REGISTER_OPERATION [](Register & lhs, Register & rhs) -> VmError { UNREFERENCED_PARAMETER(lhs); UNREFERENCED_PARAMETER(rhs); return VmError::RegisterMismatch; }

#define CreateIntegerMathOperationTableRow(lhsValueType, lhsCppType, op) \
    { \
        UNHANDLED_REGISTER_OPERATION, /* None */ \
        [](Register & lhs, Register & rhs) -> VmError { lhs.SetInteger<lhsCppType>(lhs.GetInteger<lhsCppType>() op static_cast<lhsCppType>(rhs.GetInteger<int8>()), lhsValueType); return VmError::OK; }, /* Int8 */ \
        [](Register & lhs, Register & rhs) -> VmError { lhs.SetInteger<lhsCppType>(lhs.GetInteger<lhsCppType>() op static_cast<lhsCppType>(rhs.GetInteger<int16>()), lhsValueType); return VmError::OK; }, /* Int16 */ \
        [](Register & lhs, Register & rhs) -> VmError { lhs.SetInteger<lhsCppType>(lhs.GetInteger<lhsCppType>() op static_cast<lhsCppType>(rhs.GetInteger<int32>()), lhsValueType); return VmError::OK; }, /* Int32 */ \
        [](Register & lhs, Register & rhs) -> VmError { lhs.SetInteger<lhsCppType>(lhs.GetInteger<lhsCppType>() op static_cast<lhsCppType>(rhs.GetInteger<int64>()), lhsValueType); return VmError::OK; }, /* Int64 */ \
        [](Register & lhs, Register & rhs) -> VmError { lhs.SetInteger<lhsCppType>(lhs.GetInteger<lhsCppType>() op static_cast<lhsCppType>(rhs.GetInteger<uint8>()), lhsValueType); return VmError::OK; }, /* UInt8 */ \
        [](Register & lhs, Register & rhs) -> VmError { lhs.SetInteger<lhsCppType>(lhs.GetInteger<lhsCppType>() op static_cast<lhsCppType>(rhs.GetInteger<uint16>()), lhsValueType); return VmError::OK; }, /* UInt16 */ \
        [](Register & lhs, Register & rhs) -> VmError { lhs.SetInteger<lhsCppType>(lhs.GetInteger<lhsCppType>() op static_cast<lhsCppType>(rhs.GetInteger<uint32>()), lhsValueType); return VmError::OK; }, /* UInt32 */ \
        [](Register & lhs, Register & rhs) -> VmError { lhs.SetInteger<lhsCppType>(lhs.GetInteger<lhsCppType>() op static_cast<lhsCppType>(rhs.GetInteger<uint64>()), lhsValueType); return VmError::OK; }, /* UInt64 */ \
        [](Register & lhs, Register & rhs) -> VmError { lhs.SetInteger<lhsCppType>(lhs.GetInteger<lhsCppType>() op static_cast<lhsCppType>(rhs.GetBool()), lhsValueType); return VmError::OK; }, /* Bool */ \
        UNHANDLED_REGISTER_OPERATION, /* Float */ \
        UNHANDLED_REGISTER_OPERATION, /* Double */ \
        UNHANDLED_REGISTER_OPERATION, /* NativeFunction */ \
        UNHANDLED_REGISTER_OPERATION, /* Table */ \
        UNHANDLED_REGISTER_OPERATION, /* Array */ \
    },

#define CreateFPMathOperationTableRow(lhsValueType, lhsCppType, op) \
    { \
        /* None */ \
        UNHANDLED_REGISTER_OPERATION, /* None */ \
        UNHANDLED_REGISTER_OPERATION, /* Int8 */ \
        UNHANDLED_REGISTER_OPERATION, /* Int16 */ \
        UNHANDLED_REGISTER_OPERATION, /* Int32 */ \
        UNHANDLED_REGISTER_OPERATION, /* Int64 */ \
        UNHANDLED_REGISTER_OPERATION, /* UInt8 */ \
        UNHANDLED_REGISTER_OPERATION, /* UInt16 */ \
        UNHANDLED_REGISTER_OPERATION, /* UInt32 */ \
        UNHANDLED_REGISTER_OPERATION, /* UInt64 */ \
        UNHANDLED_REGISTER_OPERATION, /* Bool */ \
        [](Register & lhs, Register & rhs) -> VmError { lhs.SetFP<lhsCppType>(lhs.GetFP<lhsCppType>() op static_cast<lhsCppType>(rhs.GetFP<float>()), lhsValueType); return VmError::OK; }, /* Float */ \
        [](Register & lhs, Register & rhs) -> VmError { lhs.SetFP<lhsCppType>(lhs.GetFP<lhsCppType>() op static_cast<lhsCppType>(rhs.GetFP<double>()), lhsValueType); return VmError::OK; }, /* Double */ \
        UNHANDLED_REGISTER_OPERATION, /* NativeFunction */ \
        UNHANDLED_REGISTER_OPERATION, /* Table */ \
        UNHANDLED_REGISTER_OPERATION, /* Array */ \
    },

#define CreateMathOperationTable(name, op) RegisterMathOperation name[static_cast<uint8>(ValueType::Max)][static_cast<uint8>(ValueType::Max)] = \
{ \
    { \
        /* None */ \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
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
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
    }, \
    CreateFPMathOperationTableRow(ValueType::Float, float, op) \
    CreateFPMathOperationTableRow(ValueType::Double, double, op) \
    { \
        /* NativeFunction */ \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
    }, \
    { \
        /* Table */ \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
    }, \
    { \
        /* Array */ \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
    }, \
};

#define CreateBitwiseMathOperationTable(name, op) RegisterMathOperation name[static_cast<uint8>(ValueType::Max)][static_cast<uint8>(ValueType::Max)] = \
{ \
    { \
        /* None */ \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
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
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
    }, \
    { \
        /* Float */ \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
    }, \
    { \
        /* Double */ \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
    }, \
    { \
        /* NativeFunction */ \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
    }, \
    { \
        /* Table */ \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
    }, \
    { \
        /* Array */ \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
        UNHANDLED_REGISTER_OPERATION, \
    }, \
};

    // Integer and float
    CreateMathOperationTable(AddRegisterOperationTable, +);
    CreateMathOperationTable(SubRegisterOperationTable, -);
    CreateMathOperationTable(MulRegisterOperationTable, *);
    CreateMathOperationTable(DivRegisterOperationTable, / );

    // Integer only
    CreateBitwiseMathOperationTable(XorRegisterOperationTable, ^);
    CreateBitwiseMathOperationTable(OrRegisterOperationTable, | );
    CreateBitwiseMathOperationTable(AndRegisterOperationTable, &);
    CreateBitwiseMathOperationTable(LSHRegisterOperationTable, << );
    CreateBitwiseMathOperationTable(RSHRegisterOperationTable, >> );
    CreateBitwiseMathOperationTable(ModRegisterOperationTable, %);
}