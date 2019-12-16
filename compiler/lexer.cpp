#include "shared/types.h"
#include "token.h"
#include "char.h"
#include "lexer.h"

using namespace compiler;

static LexicalKeyword Keywords[] =
{
    { LexicalKeywordType::kw_int8,                     "int8" },
    { LexicalKeywordType::kw_int16,                    "int16" },
    { LexicalKeywordType::kw_int32,                    "int32" },
    { LexicalKeywordType::kw_int64,                    "int64" },
    { LexicalKeywordType::kw_uint8,                    "uint8" },
    { LexicalKeywordType::kw_uint16,                   "uint16" },
    { LexicalKeywordType::kw_uint32,                   "uint32" },
    { LexicalKeywordType::kw_uint64,                   "uint64" },
    { LexicalKeywordType::kw_float,                    "float" },
    { LexicalKeywordType::kw_double,                   "double" },
    { LexicalKeywordType::kw_bool,                     "bool" },
    { LexicalKeywordType::kw_char,                     "char" },
    // { LexicalKeywordType::kw_wchar,                 "wchar" },
    { LexicalKeywordType::kw_false,                    "false" },
    { LexicalKeywordType::kw_true,                     "true" },
    { LexicalKeywordType::kw_string,                   "string" },
    // { LexicalKeywordType::kw_wstring,               "wstring" },
    { LexicalKeywordType::kw_void,                     "void" },
    // { LexicalKeywordType::kw_enum,                  "enum" },
    { LexicalKeywordType::kw_const,                    "const" },
    { LexicalKeywordType::kw_if,                       "if" },
    { LexicalKeywordType::kw_else,                     "else" },
    { LexicalKeywordType::kw_for,                      "for" },
    // { LexicalKeywordType::kw_goto,                  "goto" },
    { LexicalKeywordType::kw_do,                       "do" },
    { LexicalKeywordType::kw_while,                    "while" },
    { LexicalKeywordType::kw_switch,                   "switch" },
    { LexicalKeywordType::kw_case,                     "case" },
    { LexicalKeywordType::kw_break,                    "break" },
    { LexicalKeywordType::kw_continue,                 "continue" },
    { LexicalKeywordType::kw_default,                  "default" },
    { LexicalKeywordType::kw_return,                   "return" },
    // { LexicalKeywordType::kw_class,                 "class" },
    // { LexicalKeywordType::kw_this,                  "this" },
    // { LexicalKeywordType::kw_virtual,               "virtual" },
    { LexicalKeywordType::kw_export,                   "export" },
    { LexicalKeywordType::kw_import,                   "import" },
    { LexicalKeywordType::kw_cast,                     "cast" },
    { LexicalKeywordType::kw_sizeof,                   "sizeof" },
    // { LexicalKeywordType::kw_delete,                "delete" },
    // { LexicalKeywordType::kw_new,                   "new" },
};

static LexicalPunctuator Punctuators[] =
{
    { LexicalPunctuatorType::l_square,                 "[" },
    { LexicalPunctuatorType::r_square,                 "]" },
    { LexicalPunctuatorType::l_paren,                  "(" },
    { LexicalPunctuatorType::r_paren,                  ")" },
    { LexicalPunctuatorType::l_brace,                  "{" },
    { LexicalPunctuatorType::r_brace,                  "}" },
    { LexicalPunctuatorType::period,                   "." },
    { LexicalPunctuatorType::ellipsis,                 "..." },
    { LexicalPunctuatorType::amp,                      "&" },
    { LexicalPunctuatorType::ampamp,                   "&&" },
    { LexicalPunctuatorType::ampequal,                 "&=" },
    { LexicalPunctuatorType::star,                     "*" },
    { LexicalPunctuatorType::starequal,                "*=" },
    { LexicalPunctuatorType::plus,                     "+" },
    { LexicalPunctuatorType::plusplus,                 "++" },
    { LexicalPunctuatorType::plusequal,                "+=" },
    { LexicalPunctuatorType::minus,                    "-" },
    { LexicalPunctuatorType::arrow,                    "->" },
    { LexicalPunctuatorType::minusminus,               "--" },
    { LexicalPunctuatorType::minusequal,               "-=" },
    { LexicalPunctuatorType::tilde,                    "~" },
    { LexicalPunctuatorType::exclaim,                  "!" },
    { LexicalPunctuatorType::exclaimequal,             "!=" },
    { LexicalPunctuatorType::slash,                    "/" },
    { LexicalPunctuatorType::slashequal,               "/=" },
    { LexicalPunctuatorType::percent,                  "%" },
    { LexicalPunctuatorType::percentequal,             "%=" },
    { LexicalPunctuatorType::less,                     "<" },
    { LexicalPunctuatorType::lessless,                 "<<" },
    { LexicalPunctuatorType::lessequal,                "<=" },
    { LexicalPunctuatorType::lesslessequal,            "<<=" },
    { LexicalPunctuatorType::spaceship,                "<=>" },
    { LexicalPunctuatorType::greater,                  ">" },
    { LexicalPunctuatorType::greatergreater,           ">>" },
    { LexicalPunctuatorType::greaterequal,             ">=" },
    { LexicalPunctuatorType::greatergreaterequal,      ">>=" },
    { LexicalPunctuatorType::caret,                    "^" },
    { LexicalPunctuatorType::caretequal,               "^=" },
    { LexicalPunctuatorType::pipe,                     "|" },
    { LexicalPunctuatorType::pipepipe,                 "||" },
    { LexicalPunctuatorType::pipeequal,                "|=" },
    { LexicalPunctuatorType::question,                 "?" },
    { LexicalPunctuatorType::colon,                    ":" },
    { LexicalPunctuatorType::semi,                     ";" },
    { LexicalPunctuatorType::equal,                    "=" },
    { LexicalPunctuatorType::equalequal,               "==" },
    { LexicalPunctuatorType::comma,                    "," },
    // { PunctuatorType::hash,                  "#" },
    // { PunctuatorType::hashhash,              "##" },
    // { PunctuatorType::hashat,                "#@" },
    // { PunctuatorType::periodstar,            ".*" },
    // { PunctuatorType::arrowstar,             "->*" },
    // { PunctuatorType::coloncolon,            "::" }
};

template<typename T, uint32 N>
uint32 CountOfArray(T(&var)[N])
{
    UNREFERENCED_PARAMETER(var);

    return N;
}

Lexer::Lexer(const char* sourceText) : m_tok(sourceText)
{
    for (uint32 i = 0; i < CountOfArray(Keywords); ++i)
    {
        m_keywordLookup[Keywords[i].raw] = &Keywords[i];
    }

    for (uint32 i = 0; i < CountOfArray(Punctuators); ++i)
    {
        m_punctuatorLookup[Punctuators[i].raw] = &Punctuators[i];
    }
}

Lexer::Result Lexer::LexNumericConstant(Character& c, LexicalToken& token)
{
    uint32 startIndex = c.sourceIndex;

    while (compiler::charinfo::isPreprocessingNumberBody(c.character))
    {
        if (m_tok.GetCharacter(c) == Tokenizer::Result::EndOfSource)
            break;
    }

    // We consumed 1 extra character
    m_tok.Rewind(1);

    uint32 endIndex = c.sourceIndex;

    token.sourceLength = endIndex - startIndex;
    token.type = LexicalTokenType::NumericConstant;

    return Result::OK;
}

Lexer::Result Lexer::LexStringLiteral(Character& c, LexicalToken& token)
{
    uint32 startIndex = c.sourceIndex;

    auto result = m_tok.GetCharacter(c);

    if (result == Tokenizer::Result::EndOfSource)
    {
        // We unexpectedly hit the end of source after ", guess we should mark it as a token anyway...
        token.sourceLength = 1;
        token.type = LexicalTokenType::Unknown;
        m_tok.Rewind(1);

        return Result::InvalidToken;
    }

    while (c.character != '"')
    {
        if (m_tok.GetCharacter(c) == Tokenizer::Result::EndOfSource)
            break;
    }

    uint32 endIndex = c.sourceIndex + 1;

    token.sourceLength = endIndex - startIndex;
    token.type = LexicalTokenType::StringLiteral;

    return Result::OK;
}

Lexer::Result Lexer::LexCharConstant(Character& c, LexicalToken& token)
{
    auto r1 = m_tok.GetCharacter(c);

    if (r1 == Tokenizer::Result::EndOfSource || !charinfo::isASCII(c.character))
    {
        token.sourceLength = 1;
        token.type = LexicalTokenType::Unknown;
        return Result::InvalidToken;
    }

    auto r2 = m_tok.GetCharacter(c);

    if (r2 == Tokenizer::Result::EndOfSource || c.character != '\'')
    {
        token.sourceLength = 1;
        token.type = LexicalTokenType::Unknown;
        return Result::InvalidToken;
    }

    token.sourceLength = 3;
    token.type = LexicalTokenType::CharConstant;

    return Result::OK;
}

Lexer::Result Lexer::LexIdentifier(Character& c, LexicalToken& token)
{
    uint32 startIndex = c.sourceIndex;

    while (compiler::charinfo::isIdentifierBody(c.character))
    {
        if (m_tok.GetCharacter(c) == Tokenizer::Result::EndOfSource)
            break;
    }

    // We consumed 1 extra character
    m_tok.Rewind(1);

    uint32 endIndex = c.sourceIndex;

    token.sourceLength = endIndex - startIndex;
    token.type = LexicalTokenType::Identifier;

    // Test for keyword.
    // TODO: I'm not sure this is correct. Sure, the Lexer could determine the difference between
    // An indentifier and a keyword (such as, stringIdenfierName vs string IdentifierName)
    // but shouldn't we just do it here? This parser is already basically a lexer. It just doesn't take state into account.
    std::string total(&m_tok.GetSourceText()[startIndex], &m_tok.GetSourceText()[endIndex]);

    auto f = m_keywordLookup.find(total);
    if (f != m_keywordLookup.end())
    {
        token.type = LexicalTokenType::Keyword;
        token.kw = f->second;
    }

    return Result::OK;
}

Lexer::Result Lexer::GetToken(LexicalToken& token)
{
    Character c;
    Tokenizer::Result result = m_tok.GetCharacter(c);

    if (result == Tokenizer::Result::EndOfSource)
    {
        return Result::EndOfSource;
    }

    token.kw = nullptr;
    token.punc = nullptr;

    token.sourceIndex = c.sourceIndex;
    token.sourceLength = 1; // until this is overwritten, it's 1.
    token.lineIndex = c.line;
    token.colIndex = c.col;
    token.type = LexicalTokenType::Unknown; // until this is overwritten, it's unknown.

    if (c.type == CharacterType::NewLine ||
        c.type == CharacterType::Space ||
        c.type == CharacterType::Tab)
    {
        token.type = LexicalTokenType::Whitespace;

        return Result::OK;
    }

    switch (c.character)
    {
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
        return LexNumericConstant(c, token);
    case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
    case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
    case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U':
    case 'V': case 'W': case 'X': case 'Y': case 'Z':
    case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
    case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
    case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u':
    case 'v': case 'w': case 'x': case 'y': case 'z':
    case '_':
        return LexIdentifier(c, token);
    case '\'':
        return LexCharConstant(c, token);
    case '"':
        return LexStringLiteral(c, token);
    default:
    {
        // Punctuator lookup, char constants, string literals,

        compiler::LexicalPunctuator* punc = GetPunctuatorAtPosition(c);
        if (punc != nullptr)
        {
            token.sourceLength = static_cast<uint32>(punc->raw.length());
            token.type = LexicalTokenType::Punctuator;
            token.punc = punc;

            m_tok.Forward(token.sourceLength - 1);

            return Result::OK;
        }

        // If that fails, I think we're just erroring at this line, unknown identifier or some such thing.

        break;
    }
    }

    return Result::OK;
}

LexicalPunctuator* Lexer::GetPunctuatorAtPosition(Character& c)
{
    const char* ptr = &m_tok.GetSourceText()[c.sourceIndex];

    // Clang store punctuators in order from simple to complex
    // If we go backward, we should be fine.
    for (uint32 p = CountOfArray(Punctuators); p-- > 0;)
    {
        LexicalPunctuator& pp = Punctuators[p];

        if (memcmp(pp.raw.data(), ptr, pp.raw.length()) != 0)
        {
            continue;
        }

        // It matches
        return &pp;
    }

    return nullptr;
}

LexicalKeyword* Lexer::GetKeywordAtPosition(Character& c)
{
    const char* ptr = &m_tok.GetSourceText()[c.sourceIndex];

    for (uint32 k = 0; k < CountOfArray(Keywords); ++k)
    {
        LexicalKeyword& kk = Keywords[k];

        if (memcmp(kk.raw.data(), ptr, kk.raw.length()) != 0)
        {
            continue;
        }

        // It matches
        return &kk;
    }

    return nullptr;
}

