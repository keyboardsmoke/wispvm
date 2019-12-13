#pragma once

#include "types.h"

namespace wisp
{
    class Random
    {
    public:
        Random() = delete;
        Random(uint32 seed) : m_seed(seed), m_engine(seed)
        {

        }

        template<typename T>
        T Integer(T min = std::numeric_limits<T>::min(), T max = std::numeric_limits<T>::max())
        {
            std::uniform_int_distribution<T> dist(min, max);
            return dist(m_engine);
        }

        template<typename T>
        T Real(T min = std::numeric_limits<T>::min(), T max = std::numeric_limits<T>::max())
        {
            std::uniform_real_distribution<T> dist(min, max);
            return dist(m_engine);
        }

        template<typename T>
        void ShuffleVector(std::vector<T>& v)
        {
            std::shuffle(v.begin(), v.end(), m_engine);
        }

    private:
        uint32 m_seed;
        std::default_random_engine m_engine;
    };
}