#pragma once

#include "types.h"

namespace wisp
{
    class Decoder
    {
    public:
        template<typename T> static T Decode(T& number, uint64 key)
        {
            return number ^ key;
        }
    };
}