#pragma once

namespace compiler
{
    enum class CharacterType
    {
        General,
        Space,
        Tab,
        NewLine,
        EndOfFile
    };

    struct Character
    {
        uint32 line;
        uint32 col;
        uint32 sourceIndex;

        CharacterType type;

        char character;
    };

    class Scanner
    {
    public:
        enum class ScannerResult
        {
            OK,
            EndOfSource
        };

        Scanner() = delete;
        Scanner(const char* sourceText) : 
            m_src(sourceText), m_sourceIndex(0), m_lineIndex(0), m_colIndex(0) {}

        ScannerResult GetCharacter(Character& character);

        // This assumes we aren't advancing past newlines.
        // TODO: If we want to look for newlines, we can, but that isn't an option yet.
        void Forward(uint32 count)
        {
            m_sourceIndex += count;
            m_colIndex += count;
        }

        void Rewind(uint32 count)
        {
            m_sourceIndex -= count;
            m_colIndex -= count;
        }

        const char* GetSourceText()
        {
            return m_src;
        }

    private:
        const char* m_src;

        uint32 m_sourceIndex;
        uint32 m_lineIndex;
        uint32 m_colIndex;
    };
}