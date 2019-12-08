#pragma once

namespace wisp
{
    namespace hash
    {
        uint32 crc32_for_byte(uint32 r) {
            for(int j = 0; j < 8; ++j)
                r = (r & 1? 0: (uint32)0xEDB88320L) ^ r >> 1;
            return r ^ (uint32)0xFF000000L;
        }

        void crc32(const void *data, uint32 n_bytes, uint32* crc) {
            static uint32 table[0x100];
            if(!*table)
                for(uint32 i = 0; i < 0x100; ++i)
                    table[i] = crc32_for_byte(i);
            for(uint32 i = 0; i < n_bytes; ++i)
                *crc = table[(uint8)*crc ^ ((uint8*)data)[i]] ^ *crc >> 8;
        }

        uint32 crc32(const void* data, uint32 size)
        {
            uint8* p_data = reinterpret_cast<uint8*>(data);

            uint32 crc = 0;
            for (uint32 i = 0; i < size; ++i)
                crc32(&p_data[i], 1, &crc);

            return crc;
        }
    }
}