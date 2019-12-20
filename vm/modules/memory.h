#pragma once

#include <cassert>

namespace vmcore
{
    // The default state 
    struct PageTableEntry
    {
        // All permissions by default
        PageTableEntry() :
            Readable(1u), Writable(1u), Executable(1u) {}

        PageTableEntry(uint8 read, uint8 write, uint8 execute) :
            Readable(read), Writable(write), Executable(execute) {}

        uint8 Readable : 1;
        uint8 Writable : 1;
        uint8 Executable : 1;
    };
    static_assert(sizeof(PageTableEntry) == sizeof(uint8));

    class MemoryModule
    {
    public:
        static constexpr const uint32 PageSize = 0x1000;

        MemoryModule() = delete;
        
        MemoryModule(uint64 totalMemorySize) : m_size(totalMemorySize)
        {
            assert((m_size % PageSize) == 0);

            // Make sure to initialize these to zero.
            m_pool = new uint8[totalMemorySize]();
            m_pageTable = new PageTableEntry[(m_size / PageSize)]();
        }

        ~MemoryModule()
        { 
            delete[] m_pageTable;
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

        PageTableEntry* GetPageTableEntryForAddress(uint8* dst)
        {
            if (dst < m_pool || dst >= (m_pool + m_size))
                return nullptr;

            uint64 offset = static_cast<uint64>(dst - m_pool);
            uint64 alignedOffset = PageAlign(offset);
            assert(alignedOffset < m_size);
            return &m_pageTable[alignedOffset / PageSize];
        }

        bool MarkPagesWithPermissions(uint8* dst, uint64 size, const PageTableEntry& entry)
        {
            if (dst < m_pool || dst >= (m_pool + m_size))
                return false;

            uint64 offset = static_cast<uint64>(dst - m_pool);

            assert((offset % PageSize) == 0);
            assert((size % PageSize) == 0);

            size_t startIndex = offset / PageSize;

            for (size_t i = 0; i < (size / PageSize); ++i)
            {
                m_pageTable[startIndex + i] = entry;
            }

            return true;
        }

        void WriteMemoryWithPagePermissions(uint8* dst, uint8* src, uint64 size, const PageTableEntry& entry)
        {
            memcpy(dst, src, size);

            size = PageAlignUp(size); // We mark the entire page
            MarkPagesWithPermissions(dst, size, entry);
        }

        template<typename T>
        T ReadWithPermissions(uint8* dst)
        {
            PageTableEntry* beg = GetPageTableEntryForAddress(dst);
            PageTableEntry* end = GetPageTableEntryForAddress(dst + sizeof(T));
            assert(beg != nullptr && end != nullptr);

            if (beg == end)
            {
                assert(beg->Readable == 1u);
            }
            else
            {
                assert(beg->Readable == 1u && end->Readable == 1u);
            }

            return *reinterpret_cast<T*>(&dst[0]);
        }

        template<typename T>
        void WriteWithPermissions(uint8* dst, const T& value)
        {
            PageTableEntry* beg = GetPageTableEntryForAddress(dst);
            PageTableEntry* end = GetPageTableEntryForAddress(dst + sizeof(T));
            assert(beg != nullptr && end != nullptr);

            if (beg == end)
            {
                assert(beg->Writable == 1u);
            }
            else
            {
                assert(beg->Writable == 1u && end->Writable == 1u);
            }

            *reinterpret_cast<T*>(&dst[0]) = value;
        }

        bool CanExecuteAtLocation(uint8* dst)
        {
            PageTableEntry* pte = GetPageTableEntryForAddress(dst);
            assert(pte != nullptr);
            return (pte->Executable == 1u);
        }

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
        PageTableEntry* m_pageTable;

        // The underlying OS will take care of paging in/out memory as needed
        // but we have to take care of permissions. Every page's permissions are readable by default.
    };
}