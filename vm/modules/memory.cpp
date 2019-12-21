#include "shared/types.h"
#include "memory.h"

using namespace vmcore;

bool MemoryModule::Read(uint64 offset, void* data, uint64 size)
{
    if (!CanRead(offset, size))
        return false;

    memcpy(data, m_pool + offset, size);

    return true;
}

bool MemoryModule::Write(uint64 offset, void* data, uint64 size)
{
    if (!CanWrite(offset, size))
        return false;

    memcpy(m_pool + offset, data, size);

    return true;
}