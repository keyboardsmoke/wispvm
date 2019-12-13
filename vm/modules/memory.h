#pragma once

namespace vmcore
{
    class MemoryModule
    {
    public:
        MemoryModule() = delete;
        MemoryModule(uint64 size) : m_pool(new uint8[size]), m_size(size) {}
        ~MemoryModule() { delete[] m_pool; }

        uint8* GetPhysicalMemory()
        {
            return m_pool;
        }

        uint64 GetPhysicalMemorySize()
        {
            return m_size;
        }

    private:
        uint8* m_pool;
        uint64 m_size;
    };
}