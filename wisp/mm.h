#pragma once

#include "vm/vm.h"

namespace wisp
{
    class MemoryModule : public vmcore::MemoryModule
    {
    public:
        MemoryModule(uint8* data, uint64 size) : vmcore::MemoryModule(data, size) {}
        MemoryModule(uint64 totalMemorySize) : vmcore::MemoryModule(totalMemorySize) {}

        bool CanExecute(uint64 offset, uint64 size) override
        {
            return true;
        }

        bool CanRead(uint64 offset, uint64 size) override
        {
            return true;
        }

        bool CanWrite(uint64 offset, uint64 size) override
        {
            return true;
        }
    };
}