#pragma once

namespace compiler
{
    class Parser
    {
    public:
        Parser() = delete;
        Parser(const char* sourceText);

    private:
        Lexer m_lex;
    };
}