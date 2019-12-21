#pragma once

#include <cassert>

namespace vmcore
{
    class MemoryModule
    {
    public:
        static constexpr const uint32 PageSize = 0x1000;

        MemoryModule() = delete;

        MemoryModule(uint8* data, uint64 size) : m_size(size)
        {
            // We want to force it to be page aligned, even if they provide a lower value.
            uint64 allocationSize = PageAlignUp(size);

            // Make sure to initialize these to zero.
            m_pool = new uint8[allocationSize]();

            // Set initial data, all other reads and writes must go through interfaces
            memcpy(m_pool, data, size);
        }

        MemoryModule(uint64 totalMemorySize) : m_size(totalMemorySize)
        {
            assert((m_size % PageSize) == 0);

            // Make sure to initialize these to zero.
            m_pool = new uint8[totalMemorySize]();
        }

        ~MemoryModule()
        { 
            delete[] m_pool;
        }

        uint64 PageAlign(uint64 address) const
        {
            return (address & ~(PageSize - 1));
        }

        uint64 PageAlignUp(uint64 address) const
        {
            return PageAlign(address + (PageSize - 1));
        }

        virtual bool CanExecute(uint64 offset, uint64 size) = 0;
        virtual bool CanRead(uint64 offset, uint64 size) = 0;
        virtual bool CanWrite(uint64 offset, uint64 size) = 0;

        template<typename T>
        T Read(uint64 offset)
        {
            T ret;
            assert(Read(offset, &ret, sizeof(T)));
            return ret;
        }

        template<typename T>
        void Write(uint64 offset, const T& data)
        {
            assert(Write(offset, const_cast<T*>(&data), sizeof(T)));
        }

        virtual bool Read(uint64 offset, void* data, uint64 size);
        virtual bool Write(uint64 offset, void* data, uint64 size);

    protected:
        // Marked as protected in case a class that inherets wants to expose these
        // It isn't recommended, but if you don't want to simulate page protections
        // It's probably a lot easier.
        uint8* m_pool;
        uint64 m_size;
    };
}