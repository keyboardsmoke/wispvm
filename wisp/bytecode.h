#pragma once

#include "shared/types.h"
#include "register.h"
#include "context.h"
#include "isa/isa.h"

namespace wisp
{
	// Create to aid in the building of bytecode for testing and just kind of in general :)
	class ByteCodeGenerator
	{
	public:
		void Mov(GeneralPurposeRegisters dst, GeneralPurposeRegisters src)
		{
			m_bc.push_back(static_cast<uint8>(InstructionCodes::Move));
			m_bc.push_back(static_cast<uint8>(dst));
			m_bc.push_back(static_cast<uint8>(src));
		}
		
		void Mov(GeneralPurposeRegisters dst, IntegerValue value)
		{
			m_bc.push_back(static_cast<uint8>(InstructionCodes::MoveConstantInteger));
			m_bc.push_back(static_cast<uint8>(dst));
			std::visit([this](auto&& arg) { WriteEncodedValue(arg); }, value.GetValue());
		}

		void Mov(FloatingPointRegisters dst, FPValue value)
		{
			m_bc.push_back(static_cast<uint8>(InstructionCodes::MoveConstantFP));
			m_bc.push_back(static_cast<uint8>(dst));
			std::visit([this](auto&& arg) { WriteEncodedValue(arg); }, value.GetValue());
		}

		void Clear(GeneralPurposeRegisters dst)
		{
			m_bc.push_back(static_cast<uint8>(InstructionCodes::ClearRegister));
			m_bc.push_back(static_cast<uint8>(dst));
		}

		void Jump(IntegerValue delta)
		{
			m_bc.push_back(static_cast<uint8>(InstructionCodes::Jump));
			std::visit([this](auto&& arg) { WriteEncodedValue(arg); }, delta.GetValue());
		}

		void ConditionalJump(IntegerValue delta, ConditionCode code)
		{
			m_bc.push_back(static_cast<uint8>(InstructionCodes::ConditionalJump));
			m_bc.push_back(static_cast<uint8>(code));
			std::visit([this](auto&& arg) { WriteEncodedValue(arg); }, delta.GetValue());
		}

		void Compare(GeneralPurposeRegisters arg1, GeneralPurposeRegisters arg2)
		{
			m_bc.push_back(static_cast<uint8>(InstructionCodes::Compare));
			m_bc.push_back(static_cast<uint8>(arg1));
			m_bc.push_back(static_cast<uint8>(arg2));
		}

		void Compare(GeneralPurposeRegisters arg, IntegerValue constant)
		{
			m_bc.push_back(static_cast<uint8>(InstructionCodes::CompareConstant));
			m_bc.push_back(static_cast<uint8>(arg));
			std::visit([this](auto&& arg) { WriteEncodedValue(arg); }, constant.GetValue());
		}

		void Test(GeneralPurposeRegisters arg1, GeneralPurposeRegisters arg2)
		{
			m_bc.push_back(static_cast<uint8>(InstructionCodes::Test));
			m_bc.push_back(static_cast<uint8>(arg1));
			m_bc.push_back(static_cast<uint8>(arg2));
		}

		void Test(GeneralPurposeRegisters arg, IntegerValue constant)
		{
			m_bc.push_back(static_cast<uint8>(InstructionCodes::TestConstant));
			m_bc.push_back(static_cast<uint8>(arg));
			std::visit([this](auto&& arg) { WriteEncodedValue(arg); }, constant.GetValue());
		}

		void Halt()
		{
			m_bc.push_back(static_cast<uint8>(InstructionCodes::Halt));
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
			m_bc.push_back(static_cast<uint8>(IntegerValueType::Int8));
			WriteDataType(value);
		}

		void WriteEncodedValue(int16 value)
		{
			m_bc.push_back(static_cast<uint8>(IntegerValueType::Int16));
			WriteDataType(value);
		}

		void WriteEncodedValue(int32 value)
		{
			m_bc.push_back(static_cast<uint8>(IntegerValueType::Int32));
			WriteDataType(value);
		}

		void WriteEncodedValue(int64 value)
		{
			m_bc.push_back(static_cast<uint8>(IntegerValueType::Int64));
			WriteDataType(value);
		}

		void WriteEncodedValue(uint8 value)
		{
			m_bc.push_back(static_cast<uint8>(IntegerValueType::UInt8));
			WriteDataType(value);
		}

		void WriteEncodedValue(uint16 value)
		{
			m_bc.push_back(static_cast<uint8>(IntegerValueType::UInt16));
			WriteDataType(value);
		}

		void WriteEncodedValue(uint32 value)
		{
			m_bc.push_back(static_cast<uint8>(IntegerValueType::UInt32));
			WriteDataType(value);
		}

		void WriteEncodedValue(uint64 value)
		{
			m_bc.push_back(static_cast<uint8>(IntegerValueType::UInt64));
			WriteDataType(value);
		}

		void WriteEncodedValue(float value)
		{
			m_bc.push_back(static_cast<uint8>(FPValueType::Float));
			WriteDataType(value);
		}

		void WriteEncodedValue(double value)
		{
			m_bc.push_back(static_cast<uint8>(FPValueType::Double));
			WriteDataType(value);
		}

		std::vector<uint8> m_bc;
	};
}