#pragma once

// Wisp operands are not like intermediate operands from <internal name redacted>
// They are for storing the active register state in the State structure
// They have no ID associated with them, the ID lookup will occur in the VM
// They are just here to store what's happening _now_ and nothing else.
// When an instruction is being parsed, we won't be creating a _new_ register object
// They will already exist in the State structure, and will be updated or read accordingly
// We will need to know what type of value they've been set to
// So they will take a "Value" type that can be cast into:
//      Native Functions
//      Functions
//      Integers
//      Floats
//      Doubles
//      Arrays
//      Etc.

#include <cassert>
#include <cstring>

#include "vm/vm.h"

#include "integer_value.h"
#include "fp_value.h"

namespace wisp
{
    class RegisterFP
    {
    public:
        RegisterFP() : m_value() {}

        template<typename T>
        void Set(T value)
        {
            m_value.Set<T>(value);
        }

        template<typename T>
        T Get()
        {
            return m_value.Get<T>();
        }

    private:
        FPValue m_value;
    };

    class RegisterInt
    {
    public:
        RegisterInt() : m_value() {}

        template<typename T>
        void Set(T value)
        {
            m_value.Set<T>(value);
        }

        template<typename T>
        T Get()
        {
            return m_value.Get<T>();
        }

        void CopyValue(RegisterInt& rhs);
        void DestroyValue();

    private:
        IntegerValue m_value;
    };
}