#pragma once

#include "shared/types.h"
#include "register.h"
#include "context.h"
#include "isa/isa.h"
#include "wisp/isa/syscall.h"

namespace wisp
{
	// TODO: This is very likely going to evolve into an interface similar to LLVM's IR functions/builder
	// That means we will need an intermediate class to represent an instruction, operands, etc.
	// This will allow us to create instructions that reference data that doesn't exist yet, by later updating it's operands
	// An ability we lack at the moment.

	// Create to aid in the building of bytecode for testing and just kind of in general :)
	class ByteCodeGenerator
	{
	public:
		uint32 Mov(GeneralPurposeRegisters dst, GeneralPurposeRegisters src)
		{
			uint32 ret = static_cast<uint32>(m_bc.size());
			m_bc.push_back(static_cast<uint8>(InstructionCodes::Move));
			m_bc.push_back(static_cast<uint8>(dst));
			m_bc.push_back(static_cast<uint8>(src));
			return ret;
		}
		
		uint32 Mov(GeneralPurposeRegisters dst, Value value)
		{
			return std::visit([this, dst](auto&& arg) -> auto { return Mov(dst, arg); }, value.GetValue());
		}

		uint32 Mov(GeneralPurposeRegisters dst, IntegerValue value)
		{
			uint32 ret = static_cast<uint32>(m_bc.size());
			m_bc.push_back(static_cast<uint8>(InstructionCodes::MoveConstant));
			m_bc.push_back(static_cast<uint8>(dst));
			std::visit([this](auto&& arg) { WriteEncodedValue(arg); }, value.GetValue());
			return ret;
		}

		uint32 Mov(GeneralPurposeRegisters dst, FPValue value)
		{
			uint32 ret = static_cast<uint32>(m_bc.size());
			m_bc.push_back(static_cast<uint8>(InstructionCodes::MoveConstant));
			m_bc.push_back(static_cast<uint8>(dst));
			std::visit([this](auto&& arg) { WriteEncodedValue(arg); }, value.GetValue());
			return ret;
		}

		uint32 MovRelative(GeneralPurposeRegisters dst, IntegerValue delta)
		{
			uint32 ret = static_cast<uint32>(m_bc.size());
			m_bc.push_back(static_cast<uint8>(InstructionCodes::MoveRelative));
			m_bc.push_back(static_cast<uint8>(dst));
			std::visit([this](auto&& arg) { WriteEncodedValue(arg); }, delta.GetValue());
			return ret;
		}

		uint32 Clear(GeneralPurposeRegisters dst)
		{
			uint32 ret = static_cast<uint32>(m_bc.size());
			m_bc.push_back(static_cast<uint8>(InstructionCodes::ClearRegister));
			m_bc.push_back(static_cast<uint8>(dst));
			return ret;
		}

		uint32 Jump(IntegerValue delta)
		{
			uint32 ret = static_cast<uint32>(m_bc.size());
			m_bc.push_back(static_cast<uint8>(InstructionCodes::Jump));
			std::visit([this](auto&& arg) { WriteEncodedValue(arg); }, delta.GetValue());
			return ret;
		}

		uint32 ConditionalJump(ConditionCode code, IntegerValue delta)
		{
			uint32 ret = static_cast<uint32>(m_bc.size());
			m_bc.push_back(static_cast<uint8>(InstructionCodes::ConditionalJump));
			m_bc.push_back(static_cast<uint8>(code));
			std::visit([this](auto&& arg) { WriteEncodedValue(arg); }, delta.GetValue());
			return ret;
		}

		uint32 Compare(GeneralPurposeRegisters arg1, GeneralPurposeRegisters arg2)
		{
			uint32 ret = static_cast<uint32>(m_bc.size());
			m_bc.push_back(static_cast<uint8>(InstructionCodes::Compare));
			m_bc.push_back(static_cast<uint8>(arg1));
			m_bc.push_back(static_cast<uint8>(arg2));
			return ret;
		}

		uint32 Compare(GeneralPurposeRegisters arg, IntegerValue constant)
		{
			uint32 ret = static_cast<uint32>(m_bc.size());
			m_bc.push_back(static_cast<uint8>(InstructionCodes::CompareConstant));
			m_bc.push_back(static_cast<uint8>(arg));
			std::visit([this](auto&& arg) { WriteEncodedValue(arg); }, constant.GetValue());
			return ret;
		}

		uint32 Test(GeneralPurposeRegisters arg1, GeneralPurposeRegisters arg2)
		{
			uint32 ret = static_cast<uint32>(m_bc.size());
			m_bc.push_back(static_cast<uint8>(InstructionCodes::Test));
			m_bc.push_back(static_cast<uint8>(arg1));
			m_bc.push_back(static_cast<uint8>(arg2));
			return ret;
		}

		uint32 Test(GeneralPurposeRegisters arg, IntegerValue constant)
		{
			uint32 ret = static_cast<uint32>(m_bc.size());
			m_bc.push_back(static_cast<uint8>(InstructionCodes::TestConstant));
			m_bc.push_back(static_cast<uint8>(arg));
			std::visit([this](auto&& arg) { WriteEncodedValue(arg); }, constant.GetValue());
			return ret;
		}

		uint32 SystemCall(SystemCallIndices index)
		{
			uint32 ret = static_cast<uint32>(m_bc.size());
			m_bc.push_back(static_cast<uint8>(InstructionCodes::SystemCall));
			m_bc.push_back(static_cast<uint8>(index));
			return ret;
		}

		uint32 Halt()
		{
			uint32 ret = static_cast<uint32>(m_bc.size());
			m_bc.push_back(static_cast<uint8>(InstructionCodes::Halt));
			return ret;
		}

		// Complex stuff
		uint32 CreateString(GeneralPurposeRegisters dst, GeneralPurposeRegisters src)
		{
			uint32 ret = static_cast<uint32>(m_bc.size());
			m_bc.push_back(static_cast<uint8>(InstructionCodes::StringCreate));
			m_bc.push_back(static_cast<uint8>(dst));
			m_bc.push_back(static_cast<uint8>(src));
			return ret;
		}

		uint32 FormatString()
		{
			// That R0 is the parameter is implicit. R1-R31 (depending on entries)
			// is the format string variables.
			// If all registers end up getting used... well, oops. Need to fix later with stack.
			uint32 ret = static_cast<uint32>(m_bc.size());
			m_bc.push_back(static_cast<uint8>(InstructionCodes::StringFormat));
			return ret;
		}

		std::vector<uint8>& GetData()
		{
			return m_bc;
		}

	private:
		template<typename T>
		void WriteDataType(T value)
		{
			for (size_t i = 0; i < sizeof(T); ++i)
			{
				uint8* ptr = reinterpret_cast<uint8*>(&value);

				m_bc.push_back(ptr[i]);
			}
		}

		void WriteEncodedValue(int8 value)
		{
			m_bc.push_back(static_cast<uint8>(ValueType::Integer));
			m_bc.push_back(static_cast<uint8>(IntegerValueType::Int8));
			WriteDataType(value);
		}

		void WriteEncodedValue(int16 value)
		{
			m_bc.push_back(static_cast<uint8>(ValueType::Integer));
			m_bc.push_back(static_cast<uint8>(IntegerValueType::Int16));
			WriteDataType(value);
		}

		void WriteEncodedValue(int32 value)
		{
			m_bc.push_back(static_cast<uint8>(ValueType::Integer));
			m_bc.push_back(static_cast<uint8>(IntegerValueType::Int32));
			WriteDataType(value);
		}

		void WriteEncodedValue(int64 value)
		{
			m_bc.push_back(static_cast<uint8>(ValueType::Integer));
			m_bc.push_back(static_cast<uint8>(IntegerValueType::Int64));
			WriteDataType(value);
		}

		void WriteEncodedValue(uint8 value)
		{
			m_bc.push_back(static_cast<uint8>(ValueType::Integer));
			m_bc.push_back(static_cast<uint8>(IntegerValueType::UInt8));
			WriteDataType(value);
		}

		void WriteEncodedValue(uint16 value)
		{
			m_bc.push_back(static_cast<uint8>(ValueType::Integer));
			m_bc.push_back(static_cast<uint8>(IntegerValueType::UInt16));
			WriteDataType(value);
		}

		void WriteEncodedValue(uint32 value)
		{
			m_bc.push_back(static_cast<uint8>(ValueType::Integer));
			m_bc.push_back(static_cast<uint8>(IntegerValueType::UInt32));
			WriteDataType(value);
		}

		void WriteEncodedValue(uint64 value)
		{
			m_bc.push_back(static_cast<uint8>(ValueType::Integer));
			m_bc.push_back(static_cast<uint8>(IntegerValueType::UInt64));
			WriteDataType(value);
		}

		void WriteEncodedValue(float value)
		{
			m_bc.push_back(static_cast<uint8>(ValueType::FP));
			m_bc.push_back(static_cast<uint8>(FPValueType::Float));
			WriteDataType(value);
		}

		void WriteEncodedValue(double value)
		{
			m_bc.push_back(static_cast<uint8>(ValueType::FP));
			m_bc.push_back(static_cast<uint8>(FPValueType::Double));
			WriteDataType(value);
		}

		// Functions to take care of visitors that shouldn't ever happen

		uint32 Mov(GeneralPurposeRegisters dst, StringValue value)
		{
			assert(false);
		}

		uint32 Mov(GeneralPurposeRegisters dst, TableValue value)
		{
			assert(false);
		}

		uint32 Mov(GeneralPurposeRegisters dst, ArrayValue value)
		{
			assert(false);
		}


		std::vector<uint8> m_bc;
	};
}