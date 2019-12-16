#pragma once

namespace compiler
{
    enum class LexicalTokenType
    {
        Unknown,                    // Not a token.
        EndOfFile,                  // End of file.
        EndOfDirective,             // End of preprocessing directive(end of line inside a directive).
        Comment,                    // Comment
        Identifier,                 // abcde123
        NumericConstant,            // 0x123
        CharConstant,               // 'a'
        // WideCharConstant,           // L'a'
        StringLiteral,              // "foo"
        // WideStringLiteral,          // L"foo"
        Punctuator,                 // "[", "]", "&&", "+=", etc.
        Keyword,                    // "uint8", "import", "case", "if", "else", etc.
        Whitespace,                 // space, newline, tab
    };

    enum class LexicalPunctuatorType
    {
        l_square,                   // [
        r_square,                   // ]
        l_paren,                    // (
        r_paren,                    // )
        l_brace,                    // {
        r_brace,                    // }
        period,                     // .
        ellipsis,                   // ...
        amp,                        // &
        ampamp,                     // &&
        ampequal,                   // &=
        star,                       // *
        starequal,                  // *=
        plus,                       // +
        plusplus,                   // ++
        plusequal,                  // +=
        minus,                      // -
        arrow,                      // ->
        minusminus,                 // --
        minusequal,                 // -=
        tilde,                      // ~
        exclaim,                    // !
        exclaimequal,               // !=
        slash,                      // /
        slashequal,                 // /=
        percent,                    // %
        percentequal,               // %=
        less,                       // <
        lessless,                   // <<
        lessequal,                  // <=
        lesslessequal,              // <<=
        spaceship,                  // <=>
        greater,                    // >
        greatergreater,             // >>
        greaterequal,               // >=
        greatergreaterequal,        // >>=
        caret,                      // ^
        caretequal,                 // ^=
        pipe,                       // |
        pipepipe,                   // ||
        pipeequal,                  // |=
        question,                   // ?
        colon,                      // :
        semi,                       // ;
        equal,                      // =
        equalequal,                 // ==
        comma,                      // ,
        // hash,                    // #
        // hashhash,                // ##
        // hashat,                  // #@
        // periodstar,              // .*
        // arrowstar,               // ->*
        // coloncolon,              // ::
    };

    enum class LexicalKeywordType
    {
        kw_int8,
        kw_int16,
        kw_int32,
        kw_int64,

        kw_uint8,
        kw_uint16,
        kw_uint32,
        kw_uint64,

        kw_float,
        kw_double,

        kw_char,
        // kw_wchar,

        kw_bool,
        kw_false,
        kw_true,

        kw_string,
        // kw_wstring,

        kw_void,

        // kw_enum,

        kw_const,


        kw_if,
        kw_else,

        kw_for,
        // kw_goto,
        kw_do,
        kw_while,
        kw_switch,
        kw_case,
        kw_break,
        kw_continue,
        kw_default,
        kw_return,

        // kw_class,
        // kw_this,
        // kw_virtual,

        kw_export,
        kw_import,

        kw_cast,
        kw_sizeof,
        // kw_delete,
        // kw_new,
    };

    struct LexicalKeyword
    {
        LexicalKeyword() = delete;
        LexicalKeyword(LexicalKeywordType type, const std::string& raw) :
            type(type), raw(raw) {}

        LexicalKeywordType type;
        std::string raw;
    };

    struct LexicalPunctuator
    {
        LexicalPunctuator() = delete;
        LexicalPunctuator(LexicalPunctuatorType type, const std::string& raw) :
            type(type), raw(raw) {}

        LexicalPunctuatorType type;
        std::string raw;
    };

    struct LexicalToken
    {
        LexicalToken() :
            sourceIndex(0),
            sourceLength(0),
            lineIndex(0),
            colIndex(0),
            kw(nullptr),
            punc(nullptr),
            type(LexicalTokenType::Unknown) {}

        uint32 sourceIndex;
        uint32 sourceLength;
        uint32 lineIndex;
        uint32 colIndex;

        LexicalKeyword* kw;
        LexicalPunctuator* punc;

        LexicalTokenType type;
    };

    class Lexer
    {
    public:

        enum class Result
        {
            OK,
            InvalidToken,
            EndOfSource
        };

        Lexer() = delete;
        Lexer(const char* sourceText);

        Tokenizer& GetTokenizer()
        {
            return m_tok;
        }

        Result GetToken(LexicalToken& token);

    private:
        Result LexNumericConstant(Character& c, LexicalToken& token);
        Result LexStringLiteral(Character& c, LexicalToken& token);
        Result LexCharConstant(Character& c, LexicalToken& token);
        Result LexIdentifier(Character& c, LexicalToken& token);

        LexicalPunctuator* GetPunctuatorAtPosition(Character& c);
        LexicalKeyword* GetKeywordAtPosition(Character& c);

        Tokenizer m_tok;

        std::unordered_map<std::string, LexicalKeyword*> m_keywordLookup;
        std::unordered_map<std::string, LexicalPunctuator*> m_punctuatorLookup;
    };
}