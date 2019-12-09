#include "shared/types.h"
#include "stack.h"

using namespace wisp;

Stack::Stack(uint64 reserveSize)
{
    m_stackBase = new uint8[reserveSize] ();
    m_stackTotalSize = reserveSize;
    m_stackUsedSize = 0;
}

void Stack::Push(const uint64& value)
{
    *reinterpret_cast<uint64*>(Demand(sizeof(uint64))) = value;
}

uint64 Stack::Pop()
{
    uint64 p = 0;
    ReleaseAndCopy(&p, sizeof(uint64));
    return p;
}

uint8* Stack::Demand(uint64 size)
{
    uint8* currentTop = GetTop();
    Request(size);
    m_stackUsedSize += size;
    return currentTop;
}

void Stack::Release(uint64 size)
{
    // We never shrink the stack, maybe we could. I don't wanna.
    m_stackUsedSize -= size;
}

void Stack::ReleaseAndCopy(void* data, uint64 size)
{
    memcpy(data, GetTop() - size, size);
    Release(size);
}

void Stack::Request(uint64 size)
{
    uint64 newSize = m_stackUsedSize + size;

    if (newSize > m_stackTotalSize)
    {
        // Very simple (and not good) algorithm that will get larger as the stack does
        uint64 newGrowSize = m_stackTotalSize / 2;

        Grow(newGrowSize);
    }
}

void Stack::Grow(uint64 size)
{
    uint64 newStackTotalSize = m_stackTotalSize + size;
    uint8* newStack = new uint8[newStackTotalSize] ();
    memcpy(newStack, m_stackBase, m_stackTotalSize);
    delete[] m_stackBase;
    m_stackBase = newStack;
    m_stackTotalSize = newStackTotalSize;
}

uint8* Stack::GetTop()
{
    return m_stackBase + m_stackUsedSize;
}