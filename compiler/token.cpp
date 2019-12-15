#include "shared/types.h"
#include "scanner.h"
#include "token.h"
#include "char.h"

using namespace compiler;

static Keyword Keywords[] =
{
	{ KeywordType::kw_int8,						"int8" },
	{ KeywordType::kw_int16,					"int16" },
	{ KeywordType::kw_int32,					"int32" },
	{ KeywordType::kw_int64,					"int64" },
	{ KeywordType::kw_uint8,					"uint8" },
	{ KeywordType::kw_uint16,					"uint16" },
	{ KeywordType::kw_uint32,					"uint32" },
	{ KeywordType::kw_uint64,					"uint64" },
	{ KeywordType::kw_float,					"float" },
	{ KeywordType::kw_double,					"double" },
	{ KeywordType::kw_bool,						"bool" },
	{ KeywordType::kw_char,						"char" },
	// { KeywordType::kw_wchar,					"wchar" },
	{ KeywordType::kw_false,					"false" },
	{ KeywordType::kw_true,						"true" },
	{ KeywordType::kw_string,					"string" },
	// { KeywordType::kw_wstring,				"wstring" },
	{ KeywordType::kw_void,						"void" },
	// { KeywordType::kw_enum,					"enum" },
	{ KeywordType::kw_const,					"const" },
	{ KeywordType::kw_if,						"if" },
	{ KeywordType::kw_else,						"else" },
	{ KeywordType::kw_for,						"for" },
	// { KeywordType::kw_goto,					"goto" },
	{ KeywordType::kw_do,						"do" },
	{ KeywordType::kw_while,					"while" },
	{ KeywordType::kw_switch,					"switch" },
	{ KeywordType::kw_case,						"case" },
	{ KeywordType::kw_break,					"break" },
	{ KeywordType::kw_continue,					"continue" },
	{ KeywordType::kw_default,					"default" },
	{ KeywordType::kw_return,					"return" },
	// { KeywordType::kw_class,					"class" },
	// { KeywordType::kw_this,					"this" },
	// { KeywordType::kw_virtual,				"virtual" },
	{ KeywordType::kw_export,					"export" },
	{ KeywordType::kw_import,					"import" },
	{ KeywordType::kw_cast,						"cast" },
	{ KeywordType::kw_sizeof,					"sizeof" },
	// { KeywordType::kw_delete,				"delete" },
	// { KeywordType::kw_new,					"new" },
};

static Punctuator Punctuators[] =
{
	{ PunctuatorType::l_square,					"[" },
	{ PunctuatorType::r_square,					"]" },
	{ PunctuatorType::l_paren,					"(" },
	{ PunctuatorType::r_paren,					")" },
	{ PunctuatorType::l_brace,					"{" },
	{ PunctuatorType::r_brace,					"}" },
	{ PunctuatorType::period,					"." },
	{ PunctuatorType::ellipsis,					"..." },
	{ PunctuatorType::amp,						"&" },
	{ PunctuatorType::ampamp,					"&&" },
	{ PunctuatorType::ampequal,					"&=" },
	{ PunctuatorType::star,						"*" },
	{ PunctuatorType::starequal,				"*=" },
	{ PunctuatorType::plus,						"+" },
	{ PunctuatorType::plusplus,					"++" },
	{ PunctuatorType::plusequal,				"+=" },
	{ PunctuatorType::minus,					"-" },
	{ PunctuatorType::arrow,					"->" },
	{ PunctuatorType::minusminus,				"--" },
	{ PunctuatorType::minusequal,				"-=" },
	{ PunctuatorType::tilde,					"~" },
	{ PunctuatorType::exclaim,					"!" },
	{ PunctuatorType::exclaimequal,				"!=" },
	{ PunctuatorType::slash,					"/" },
	{ PunctuatorType::slashequal,				"/=" },
	{ PunctuatorType::percent,					"%" },
	{ PunctuatorType::percentequal,				"%=" },
	{ PunctuatorType::less,						"<" },
	{ PunctuatorType::lessless,					"<<" },
	{ PunctuatorType::lessequal,				"<=" },
	{ PunctuatorType::lesslessequal,			"<<=" },
	{ PunctuatorType::spaceship,				"<=>" },
	{ PunctuatorType::greater,					">" },
	{ PunctuatorType::greatergreater,			">>" },
	{ PunctuatorType::greaterequal,				">=" },
	{ PunctuatorType::greatergreaterequal,		">>=" },
	{ PunctuatorType::caret,					"^" },
	{ PunctuatorType::caretequal,				"^=" },
	{ PunctuatorType::pipe,						"|" },
	{ PunctuatorType::pipepipe,					"||" },
	{ PunctuatorType::pipeequal,				"|=" },
	{ PunctuatorType::question,					"?" },
	{ PunctuatorType::colon,					":" },
	{ PunctuatorType::semi,						";" },
	{ PunctuatorType::equal,					"=" },
	{ PunctuatorType::equalequal,				"==" },
	{ PunctuatorType::comma,					"," },
	// { PunctuatorType::hash,					"#" },
	// { PunctuatorType::hashhash,				"##" },
	// { PunctuatorType::hashat,				"#@" },
	// { PunctuatorType::periodstar,			".*" },
	// { PunctuatorType::arrowstar,				"->*" },
	// { PunctuatorType::coloncolon,			"::" }
};

template<typename T, uint32 N>
uint32 CountOfArray(T(&var)[N])
{
	UNREFERENCED_PARAMETER(var);

	return N;
}

/*
static void GetTokenType(const char* ptr, TokenType& type, uint32& length)
{
	UNREFERENCED_PARAMETER(ptr);
	UNREFERENCED_PARAMETER(type);
	UNREFERENCED_PARAMETER(length);
}
*/

Tokenizer::Tokenizer(const char* sourceText) : m_scanner(sourceText), m_modeFlags(0)
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

Tokenizer::TokenizerResult Tokenizer::GetNumericConstant(Character& c, Token& token)
{
	uint32 startIndex = c.sourceIndex;

	while (compiler::charinfo::isPreprocessingNumberBody(c.character))
	{
		if (m_scanner.GetCharacter(c) == compiler::Scanner::ScannerResult::EndOfSource)
			break;
	}

	// We consumed 1 extra character
	m_scanner.Rewind(1);

	uint32 endIndex = c.sourceIndex;

	token.sourceLength = endIndex - startIndex;
	token.type = TokenType::NumericConstant;

	return TokenizerResult::OK;
}

Tokenizer::TokenizerResult Tokenizer::GetStringLiteral(Character& c, Token& token)
{
	uint32 startIndex = c.sourceIndex;

	auto result = m_scanner.GetCharacter(c);

	if (result == Scanner::ScannerResult::EndOfSource)
	{
		// We unexpectedly hit the end of source after ", guess we should mark it as a token anyway...
		token.sourceLength = 1;
		token.type = TokenType::Unknown;
		m_scanner.Rewind(1);

		return TokenizerResult::InvalidToken;
	}

	while (c.character != '"')
	{
		if (m_scanner.GetCharacter(c) == compiler::Scanner::ScannerResult::EndOfSource)
			break;
	}

	uint32 endIndex = c.sourceIndex + 1;

	token.sourceLength = endIndex - startIndex;
	token.type = TokenType::StringLiteral;

	return TokenizerResult::OK;
}

Tokenizer::TokenizerResult Tokenizer::GetCharConstant(Character& c, Token& token)
{
	auto r1 = m_scanner.GetCharacter(c);

	if (r1 == Scanner::ScannerResult::EndOfSource || !charinfo::isASCII(c.character))
	{
		token.sourceLength = 1;
		token.type = TokenType::Unknown;
		return TokenizerResult::InvalidToken;
	}

	auto r2 = m_scanner.GetCharacter(c);

	if (r2 == Scanner::ScannerResult::EndOfSource || c.character != '\'')
	{
		token.sourceLength = 1;
		token.type = TokenType::Unknown;
		return TokenizerResult::InvalidToken;
	}

	token.sourceLength = 3;
	token.type = TokenType::CharConstant;

	return TokenizerResult::OK;
}

Tokenizer::TokenizerResult Tokenizer::GetIdentifier(Character& c, Token& token)
{
	// Test keyword first
	compiler::Keyword* kw = GetKeywordAtPosition(c);
	if (kw != nullptr)
	{
		m_scanner.Forward(static_cast<uint32>(kw->raw.length()) - 1);
		token.sourceLength = static_cast<uint32>(kw->raw.length());
		token.type = TokenType::Keyword;

		return TokenizerResult::OK;
	}

	// Do we need to do some kind of lookup here?...

	return TokenizerResult::OK;
}

#include <iostream>

Tokenizer::TokenizerResult Tokenizer::GetToken(Token& token)
{
	Character c;
	Scanner::ScannerResult result = m_scanner.GetCharacter(c);

	if (result == Scanner::ScannerResult::EndOfSource)
	{
		return TokenizerResult::EndOfSource;
	}

	token.sourceIndex = c.sourceIndex;
	token.sourceLength = 1; // until this is overwritten, it's 1.
	token.lineIndex = c.line;
	token.colIndex = c.col;
	token.type = TokenType::Unknown; // until this is overwritten, it's unknown.

	if (c.type == CharacterType::NewLine ||
		c.type == CharacterType::Space ||
		c.type == CharacterType::Tab)
	{
		token.type = TokenType::Whitespace;

		return TokenizerResult::OK;
	}

	switch (c.character)
	{
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
		return GetNumericConstant(c, token);
	case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
	case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
	case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U':
	case 'V': case 'W': case 'X': case 'Y': case 'Z':
	case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
	case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
	case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u':
	case 'v': case 'w': case 'x': case 'y': case 'z':
	case '_':
		return GetIdentifier(c, token);
	case '\'':
		return GetCharConstant(c, token);
	case '"':
		return GetStringLiteral(c, token);
	default:
	{
		// Punctuator lookup, char constants, string literals,

		compiler::Punctuator* punc = GetPunctuatorAtPosition(c);
		if (punc != nullptr)
		{
			token.sourceLength = static_cast<uint32>(punc->raw.length());
			token.type = TokenType::Punctuator;

			return TokenizerResult::OK;
		}

		// If that fails, I think we're just erroring at this line, unknown identifier or some such thing.

		break;
	}
	}

	return TokenizerResult::OK;
}

Punctuator* Tokenizer::GetPunctuatorAtPosition(Character& c)
{
	const char* ptr = &m_scanner.GetSourceText()[c.sourceIndex];

	for (uint32 p = 0; p < CountOfArray(Punctuators); ++p)
	{
		Punctuator& pp = Punctuators[p];

		if (memcmp(pp.raw.data(), ptr, pp.raw.length()) != 0)
		{
			continue;
		}

		// It matches
		return &pp;
	}

	return nullptr;
}

Keyword* Tokenizer::GetKeywordAtPosition(Character& c)
{
	const char* ptr = &m_scanner.GetSourceText()[c.sourceIndex];

	for (uint32 k = 0; k < CountOfArray(Keywords); ++k)
	{
		Keyword& kk = Keywords[k];

		if (memcmp(kk.raw.data(), ptr, kk.raw.length()) != 0)
		{
			continue;
		}

		// It matches
		return &kk;
	}

	return nullptr;
}