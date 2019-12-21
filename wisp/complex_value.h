#pragma once

#include "shared/types.h"
#include <cassert>

namespace wisp
{
    enum class ComplexValueType
    {
        None = 0,
        String,
        Table,
        Array,
    };

    class ComplexInternalType
    {
    public:
        virtual ComplexValueType GetType() = 0;
        virtual uint32 GetSize() = 0;
        virtual uint32 GetLength() = 0;

    private:
    };

    class ComplexString : public ComplexInternalType
    {
    public:
        std::string& GetString() { return m_string; }

    private:
        std::string m_string;
    };

    // TODO: Just now realizing this isn't really possible without "Value" polymorphism.
    // I mean, it is, it's just too big a pain in the ass. So I'll probably switch to that later.
    class ComplexTable : public ComplexInternalType
    {
    public:


    private:

    };

    // TODO: Ditto...
    class ComplexArray : public ComplexInternalType
    {
    public:

    private:
    };

    class ComplexValue
    {
    public:
        ComplexValue() : m_hasValue(false), m_index(0u) {}
        ComplexValue(const uint64& index) : m_hasValue(true), m_index(index)
        {
        }

        bool HasValue()
        {
            return m_hasValue;
        }

        bool IsString()
        {
            return (GetType() == ComplexValueType::String);
        }

        bool IsTable()
        {
            return (GetType() == ComplexValueType::Table);
        }

        bool IsArray()
        {
            return (GetType() == ComplexValueType::Array);
        }

        // Setters
        void Clear()
        {
            m_hasValue = false;
            m_index = 0u;
        }

        void SetIndex(uint64 index)
        {
            m_hasValue = true;
            m_index = index;
        }

        // Getters
        uint64 GetIndex()
        {
            assert(HasValue());
            return m_index;
        }

        ComplexValueType GetType()
        {
            if (!HasValue())
                return ComplexValueType::None;

            // TODO: When the complex type table exists, this will be possible.
            // Till then...
            return ComplexValueType::None;

            // return static_cast<ComplexValueType>(m_value.index() + 1);
        }

    private:
        bool m_hasValue;
        uint64 m_index;
    };
}