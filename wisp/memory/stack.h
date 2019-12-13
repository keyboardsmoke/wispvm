#pragma once

namespace wisp
{
    class Stack
    {
    public:
        Stack() : Stack(0x4000) {};
        Stack(uint64 reserveSize);

        uint8* GetStackBase()
        {
            return m_stackBase;
        }

        void Push(const uint64& value);
        uint64 Pop();

        uint8* Demand(uint64 size);
        void Release(uint64 size);
        void ReleaseAndCopy(void* data, uint64 size);

        void Request(uint64 size);
        void Grow(uint64 size);

        uint8* GetTop();

    private:
        uint8* m_stackBase;
        uint64 m_stackTotalSize;
        uint64 m_stackUsedSize;
    };
}