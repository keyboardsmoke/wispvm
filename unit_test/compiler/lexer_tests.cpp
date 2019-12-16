#include "doctest/doctest.h"
#include <unordered_map>
#include "shared/types.h"
#include "compiler/token.h"
#include "compiler/lexer.h"
#include <string>
#include <fstream>
#include <streambuf>
#include <iostream>

static std::string ReadEntireFile(const std::string& filename)
{
    std::ifstream ifs(filename, std::ios::binary);
    return std::string((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
}

static std::string TokenTypeToString(compiler::LexicalTokenType& type)
{
    switch (type)
    {
    case compiler::LexicalTokenType::EndOfFile:
        return "EndOfFile";
    case compiler::LexicalTokenType::EndOfDirective:
        return "EndOfDirective";
    case compiler::LexicalTokenType::Comment:
        return "Comment";
    case compiler::LexicalTokenType::Identifier:
        return "Identifier";
    case compiler::LexicalTokenType::NumericConstant:
        return "NumericConstant";
    case compiler::LexicalTokenType::CharConstant:
        return "CharConstant";
    case compiler::LexicalTokenType::StringLiteral:
        return "StringLiteral";
    case compiler::LexicalTokenType::Punctuator:
        return "Punctuator";
    case compiler::LexicalTokenType::Keyword:
        return "Keyword";
    case compiler::LexicalTokenType::Whitespace:
        return "Whitespace";
    }

    return "Unknown";
}

void DumpToken(const char* src, compiler::LexicalToken& token)
{
    std::string text(&src[token.sourceIndex], &src[token.sourceIndex + token.sourceLength]);

    std::cout << "Token [" << text << "][type: " << TokenTypeToString(token.type) << "]" << std::endl;
}

TEST_CASE("Lexer Tests")
{
    // TODO: We don't yet support wchar, wstring. Not sure how to handle these yet.
    // TODO: We don't enforce any grammar rules, so keyword after keyword without whitespace/punctuation is accepted. 
    //      Is this a lexer's job?

    SUBCASE("String variable and literal")
    {
        const char* fileContent = "string p = \"Hello, World!\";";

        compiler::LexicalToken token;
        compiler::Lexer lex(fileContent);

        REQUIRE(lex.GetToken(token) == compiler::Lexer::Result::OK);
        REQUIRE(token.lineIndex == 0);
        REQUIRE(token.colIndex == 0);
        REQUIRE(token.sourceIndex == 0);
        REQUIRE(token.sourceLength == 6);
        REQUIRE(token.type == compiler::LexicalTokenType::Keyword);
        REQUIRE(token.kw != nullptr);
        REQUIRE(token.kw->type == compiler::LexicalKeywordType::kw_string);
        REQUIRE(memcmp(&fileContent[token.sourceIndex], "string", 6) == 0);

        REQUIRE(lex.GetToken(token) == compiler::Lexer::Result::OK);
        REQUIRE(token.lineIndex == 0);
        REQUIRE(token.colIndex == 6);
        REQUIRE(token.sourceIndex == 6);
        REQUIRE(token.sourceLength == 1);
        REQUIRE(token.type == compiler::LexicalTokenType::Whitespace);
        REQUIRE(fileContent[token.sourceIndex] == ' ');

        REQUIRE(lex.GetToken(token) == compiler::Lexer::Result::OK);
        REQUIRE(token.lineIndex == 0);
        REQUIRE(token.colIndex == 7);
        REQUIRE(token.sourceIndex == 7);
        REQUIRE(token.sourceLength == 1);
        REQUIRE(token.type == compiler::LexicalTokenType::Identifier);
        REQUIRE(fileContent[token.sourceIndex] == 'p');

        REQUIRE(lex.GetToken(token) == compiler::Lexer::Result::OK);
        REQUIRE(token.lineIndex == 0);
        REQUIRE(token.colIndex == 8);
        REQUIRE(token.sourceIndex == 8);
        REQUIRE(token.sourceLength == 1);
        REQUIRE(token.type == compiler::LexicalTokenType::Whitespace);
        REQUIRE(fileContent[token.sourceIndex] == ' ');

        REQUIRE(lex.GetToken(token) == compiler::Lexer::Result::OK);
        REQUIRE(token.lineIndex == 0);
        REQUIRE(token.colIndex == 9);
        REQUIRE(token.sourceIndex == 9);
        REQUIRE(token.sourceLength == 1);
        REQUIRE(token.type == compiler::LexicalTokenType::Punctuator);
        REQUIRE(token.punc != nullptr);
        REQUIRE(token.punc->type == compiler::LexicalPunctuatorType::equal);
        REQUIRE(fileContent[token.sourceIndex] == '=');

        REQUIRE(lex.GetToken(token) == compiler::Lexer::Result::OK);
        REQUIRE(token.lineIndex == 0);
        REQUIRE(token.colIndex == 10);
        REQUIRE(token.sourceIndex == 10);
        REQUIRE(token.sourceLength == 1);
        REQUIRE(token.type == compiler::LexicalTokenType::Whitespace);
        REQUIRE(fileContent[token.sourceIndex] == ' ');

        REQUIRE(lex.GetToken(token) == compiler::Lexer::Result::OK);
        REQUIRE(token.lineIndex == 0);
        REQUIRE(token.colIndex == 11);
        REQUIRE(token.sourceIndex == 11);
        REQUIRE(token.sourceLength == 15);
        REQUIRE(token.type == compiler::LexicalTokenType::StringLiteral);
        REQUIRE(memcmp(&fileContent[token.sourceIndex], "\"Hello, World!\"", 15) == 0);

        REQUIRE(lex.GetToken(token) == compiler::Lexer::Result::OK);
        REQUIRE(token.lineIndex == 0);
        REQUIRE(token.colIndex == 26);
        REQUIRE(token.sourceIndex == 26);
        REQUIRE(token.sourceLength == 1);
        REQUIRE(token.type == compiler::LexicalTokenType::Punctuator);
        REQUIRE(token.punc != nullptr);
        REQUIRE(token.punc->type == compiler::LexicalPunctuatorType::semi);
        REQUIRE(fileContent[token.sourceIndex] == ';');
    }

    SUBCASE("Char Constant")
    {
        const char* fileContent = "'A';";

        compiler::LexicalToken token;
        compiler::Lexer lex(fileContent);

        REQUIRE(lex.GetToken(token) == compiler::Lexer::Result::OK);
        REQUIRE(token.lineIndex == 0);
        REQUIRE(token.colIndex == 0);
        REQUIRE(token.sourceIndex == 0);
        REQUIRE(token.sourceLength == 3);
        REQUIRE(token.type == compiler::LexicalTokenType::CharConstant);
        REQUIRE(memcmp(&fileContent[token.sourceIndex], "'A'", 3) == 0);
    }

    SUBCASE("Invalid Identifer")
    {
        const char* fileContent = "string $$;";

        compiler::LexicalToken token;
        compiler::Lexer lex(fileContent);

        REQUIRE(lex.GetToken(token) == compiler::Lexer::Result::OK);
        REQUIRE(token.lineIndex == 0);
        REQUIRE(token.colIndex == 0);
        REQUIRE(token.sourceIndex == 0);
        REQUIRE(token.sourceLength == 6);
        REQUIRE(token.type == compiler::LexicalTokenType::Keyword);
        REQUIRE(token.kw != nullptr);
        REQUIRE(token.kw->type == compiler::LexicalKeywordType::kw_string);
        REQUIRE(memcmp(&fileContent[token.sourceIndex], "string", 6) == 0);

        REQUIRE(lex.GetToken(token) == compiler::Lexer::Result::OK);
        REQUIRE(token.lineIndex == 0);
        REQUIRE(token.colIndex == 6);
        REQUIRE(token.sourceIndex == 6);
        REQUIRE(token.sourceLength == 1);
        REQUIRE(token.type == compiler::LexicalTokenType::Whitespace);
        REQUIRE(fileContent[token.sourceIndex] == ' ');

        REQUIRE(lex.GetToken(token) == compiler::Lexer::Result::OK);
        REQUIRE(token.type == compiler::LexicalTokenType::Unknown); // $
        REQUIRE(lex.GetToken(token) == compiler::Lexer::Result::OK);
        REQUIRE(token.type == compiler::LexicalTokenType::Unknown); // $
    }

    SUBCASE("Match complex punctuator first")
    {
        const char* fileContent = "&& += + -=";

        compiler::LexicalToken token;
        compiler::Lexer lex(fileContent);

        REQUIRE(lex.GetToken(token) == compiler::Lexer::Result::OK);
        REQUIRE(token.lineIndex == 0);
        REQUIRE(token.colIndex == 0);
        REQUIRE(token.sourceIndex == 0);
        REQUIRE(token.sourceLength == 2);
        REQUIRE(token.type == compiler::LexicalTokenType::Punctuator);
        REQUIRE(token.punc != nullptr);
        REQUIRE(token.punc->type == compiler::LexicalPunctuatorType::ampamp);
        REQUIRE(token.punc->raw == "&&");

        // whitespace
        REQUIRE(lex.GetToken(token) == compiler::Lexer::Result::OK);
        REQUIRE(token.type == compiler::LexicalTokenType::Whitespace);

        REQUIRE(lex.GetToken(token) == compiler::Lexer::Result::OK);
        REQUIRE(token.lineIndex == 0);
        REQUIRE(token.colIndex == 3);
        REQUIRE(token.sourceIndex == 3);
        REQUIRE(token.sourceLength == 2);
        REQUIRE(token.type == compiler::LexicalTokenType::Punctuator);
        REQUIRE(token.punc != nullptr);
        REQUIRE(token.punc->type == compiler::LexicalPunctuatorType::plusequal);
        REQUIRE(token.punc->raw == "+=");

        // whitespace
        REQUIRE(lex.GetToken(token) == compiler::Lexer::Result::OK);
        REQUIRE(token.type == compiler::LexicalTokenType::Whitespace);

        REQUIRE(lex.GetToken(token) == compiler::Lexer::Result::OK);
        REQUIRE(token.lineIndex == 0);
        REQUIRE(token.colIndex == 6);
        REQUIRE(token.sourceIndex == 6);
        REQUIRE(token.sourceLength == 1);
        REQUIRE(token.type == compiler::LexicalTokenType::Punctuator);
        REQUIRE(token.punc != nullptr);
        REQUIRE(token.punc->type == compiler::LexicalPunctuatorType::plus);
        REQUIRE(token.punc->raw == "+");

        // whitespace
        REQUIRE(lex.GetToken(token) == compiler::Lexer::Result::OK);
        REQUIRE(token.type == compiler::LexicalTokenType::Whitespace);

        REQUIRE(lex.GetToken(token) == compiler::Lexer::Result::OK);
        REQUIRE(token.lineIndex == 0);
        REQUIRE(token.colIndex == 8);
        REQUIRE(token.sourceIndex == 8);
        REQUIRE(token.sourceLength == 2);
        REQUIRE(token.type == compiler::LexicalTokenType::Punctuator);
        REQUIRE(token.punc != nullptr);
        REQUIRE(token.punc->type == compiler::LexicalPunctuatorType::minusequal);
        REQUIRE(token.punc->raw == "-=");
    }

    SUBCASE("Simple array declaration")
    {
        const char* fileContent = "uint64 baby_yoda[256];";

        compiler::LexicalToken token;
        compiler::Lexer lex(fileContent);

        REQUIRE(lex.GetToken(token) == compiler::Lexer::Result::OK);
        REQUIRE(token.lineIndex == 0);
        REQUIRE(token.colIndex == 0);
        REQUIRE(token.sourceIndex == 0);
        REQUIRE(token.sourceLength == 6);
        REQUIRE(token.type == compiler::LexicalTokenType::Keyword);
        REQUIRE(token.kw != nullptr);
        REQUIRE(token.kw->type == compiler::LexicalKeywordType::kw_uint64);
        REQUIRE(token.kw->raw == "uint64");

        // whitespace
        REQUIRE(lex.GetToken(token) == compiler::Lexer::Result::OK);
        REQUIRE(token.type == compiler::LexicalTokenType::Whitespace);

        REQUIRE(lex.GetToken(token) == compiler::Lexer::Result::OK);
        REQUIRE(token.lineIndex == 0);
        REQUIRE(token.colIndex == 7);
        REQUIRE(token.sourceIndex == 7);
        REQUIRE(token.sourceLength == 9);
        REQUIRE(token.type == compiler::LexicalTokenType::Identifier);
        REQUIRE(memcmp(&fileContent[token.sourceIndex], "baby_yoda", 9) == 0);

        REQUIRE(lex.GetToken(token) == compiler::Lexer::Result::OK);
        REQUIRE(token.lineIndex == 0);
        REQUIRE(token.colIndex == 16);
        REQUIRE(token.sourceIndex == 16);
        REQUIRE(token.sourceLength == 1);
        REQUIRE(token.type == compiler::LexicalTokenType::Punctuator);
        REQUIRE(token.punc != nullptr);
        REQUIRE(token.punc->type == compiler::LexicalPunctuatorType::l_square);
        REQUIRE(token.punc->raw == "[");

        REQUIRE(lex.GetToken(token) == compiler::Lexer::Result::OK);
        REQUIRE(token.lineIndex == 0);
        REQUIRE(token.colIndex == 17);
        REQUIRE(token.sourceIndex == 17);
        REQUIRE(token.sourceLength == 3);
        REQUIRE(token.type == compiler::LexicalTokenType::NumericConstant);
        REQUIRE(memcmp(&fileContent[token.sourceIndex], "256", 3) == 0);

        REQUIRE(lex.GetToken(token) == compiler::Lexer::Result::OK);
        REQUIRE(token.lineIndex == 0);
        REQUIRE(token.colIndex == 20);
        REQUIRE(token.sourceIndex == 20);
        REQUIRE(token.sourceLength == 1);
        REQUIRE(token.type == compiler::LexicalTokenType::Punctuator);
        REQUIRE(token.punc != nullptr);
        REQUIRE(token.punc->type == compiler::LexicalPunctuatorType::r_square);
        REQUIRE(token.punc->raw == "]");

        REQUIRE(lex.GetToken(token) == compiler::Lexer::Result::OK);
        REQUIRE(token.lineIndex == 0);
        REQUIRE(token.colIndex == 21);
        REQUIRE(token.sourceIndex == 21);
        REQUIRE(token.sourceLength == 1);
        REQUIRE(token.type == compiler::LexicalTokenType::Punctuator);
        REQUIRE(token.punc != nullptr);
        REQUIRE(token.punc->type == compiler::LexicalPunctuatorType::semi);
        REQUIRE(token.punc->raw == ";");
    }

    SUBCASE("Tricky identifiers")
    {
        const char* fileContent = R"(
            uint8 uint64p = 0;
            float floatBeep = 1.0f;
            uint64 stringFoo = 0x12345;
        )";

        compiler::LexicalToken token;
        compiler::Lexer lex(fileContent);

        while (lex.GetToken(token) != compiler::Lexer::Result::EndOfSource)
        {
            DumpToken(fileContent, token);
        }
    }
}