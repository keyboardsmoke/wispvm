#pragma once

#include "types.h"

namespace wisp
{
    class Decoder
    {
    public:
        template<typename T> static T Decode(T& number, uint32 key)
        {
            if (key == 0)
                return number;
            
            return static_cast<T>(number ^ key);
        }
    };
}