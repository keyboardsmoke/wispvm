#pragma once

namespace compiler
{
    enum class TokenType
    {
        Unknown,                    // Not a token.
        EndOfFile,                  // End of file.
        EndOfDirective,             // End of preprocessing directive(end of line inside a directive).
        CodeCompletion,             // Code completion marker
        Comment,                    // Comment
        Identifier,                 // abcde123
        RawIdentifier,              // Used only in raw lexing mode.
        NumericConstant,            // 0x123
        CharConstant,               // 'a'
        // WideCharConstant,           // L'a'
        StringLiteral,              // "foo"
        // WideStringLiteral,          // L"foo"
        Punctuator,                 // "[", "]", "&&", "+=", etc.
        Keyword,                    // "uint8", "import", "case", "if", "else", etc.
        Whitespace,                 // space, newline, tab
    };

    enum class PunctuatorType
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

    enum class KeywordType
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

    struct Keyword
    {
        Keyword() = delete;
        Keyword(KeywordType type, const std::string& raw) :
            type(type), raw(raw) {}

        KeywordType type;
        std::string raw;
    };

    struct Punctuator
    {
        Punctuator() = delete;
        Punctuator(PunctuatorType type, const std::string& raw) :
            type(type), raw(raw) {}

        PunctuatorType type;
        std::string raw;
    };

    struct Token
    {
        Token() :
            sourceIndex(0),
            sourceLength(0),
            lineIndex(0),
            colIndex(0),
            kw(nullptr),
            punc(nullptr),
            type(TokenType::Unknown) {}

        uint32 sourceIndex;
        uint32 sourceLength;
        uint32 lineIndex;
        uint32 colIndex;

        Keyword* kw;
        Punctuator* punc;

        TokenType type;
    };

    class Tokenizer
    {
    public:
        enum class TokenizerResult
        {
            OK,
            InvalidToken,
            EndOfSource
        };

        enum TokenMode : uint32
        {
            KeepWhiteSpace = 1 << 0,
            CommentRetention = 1 << 1
        };

        Tokenizer() = delete;
        Tokenizer(const char* sourceText);

        void SetTokenModeFlags(uint32 flags)
        {
            m_modeFlags = flags;
        }

        Scanner& GetScanner()
        {
            return m_scanner;
        }

        TokenizerResult GetToken(Token& token);

    private:
        TokenizerResult GetNumericConstant(Character& c, Token& token);
        TokenizerResult GetStringLiteral(Character& c, Token& token);
        TokenizerResult GetCharConstant(Character& c, Token& token);
        TokenizerResult GetIdentifier(Character& c, Token& token);

        Punctuator* GetPunctuatorAtPosition(Character& c);
        Keyword* GetKeywordAtPosition(Character& c);

        Scanner m_scanner;
        uint32 m_modeFlags;

        std::unordered_map<std::string, Keyword*> m_keywordLookup;
        std::unordered_map<std::string, Punctuator*> m_punctuatorLookup;
    };
}