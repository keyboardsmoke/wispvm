#pragma once

#include <string>
#include <unordered_map>

namespace wisp
{
    class StringFormatter
    {
    public:
        StringFormatter(const std::string fmt) : m_fmt(fmt) {}

        std::string GetFormattedString(WispContext* context);

    private:
        std::string m_fmt;
    };
}