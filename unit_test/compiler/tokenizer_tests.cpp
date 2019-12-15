#include "doctest/doctest.h"
#include <unordered_map>
#include "shared/types.h"
#include "compiler/scanner.h"
#include "compiler/token.h"
#include <string>
#include <fstream>
#include <streambuf>
#include <iostream>

static std::string ReadEntireFile(const std::string& filename)
{
	std::ifstream ifs(filename, std::ios::binary);
	return std::string((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
}

static std::string TokenTypeToString(compiler::TokenType& type)
{
	switch (type)
	{
	case compiler::TokenType::EndOfFile:
		return "EndOfFile";
	case compiler::TokenType::EndOfDirective:
		return "EndOfDirective";
	case compiler::TokenType::CodeCompletion:
		return "CodeCompletion";
	case compiler::TokenType::Comment:
		return "Comment";
	case compiler::TokenType::Identifier:
		return "Identifier";
	case compiler::TokenType::RawIdentifier:
		return "RawIdentifier";
	case compiler::TokenType::NumericConstant:
		return "NumericConstant";
	case compiler::TokenType::CharConstant:
		return "CharConstant";
	case compiler::TokenType::WideCharConstant:
		return "WideCharConstant";
	case compiler::TokenType::StringLiteral:
		return "StringLiteral";
	case compiler::TokenType::WideStringLiteral:
		return "WideStringLiteral";
	case compiler::TokenType::Punctuator:
		return "Punctuator";
	case compiler::TokenType::Keyword:
		return "Keyword";
	case compiler::TokenType::Whitespace:
		return "Whitespace";
	}

	return "Unknown";
}

TEST_CASE("Tokenizer Tests")
{
	// TODO: We don't yet support wchar, wstring. Not sure how to handle these yet.
	// TODO: We don't yet support identifiers. This is kind of an issue, yeah.
	// TODO: We don't enforce any grammar rules, so keyword after keyword without whitespace/punctuation is accepted. 
	//		Is this a lexer's job?

	SUBCASE("String variable and literal")
	{
		const char* fileContent = "string p = \"Hello, World!\";";

		compiler::Token token;
		compiler::Tokenizer tokenizer(fileContent);

		REQUIRE(tokenizer.GetToken(token) == compiler::Tokenizer::TokenizerResult::OK);
		REQUIRE(token.lineIndex == 0);
		REQUIRE(token.colIndex == 0);
		REQUIRE(token.sourceIndex == 0);
		REQUIRE(token.sourceLength == 6);
		REQUIRE(token.type == compiler::TokenType::Keyword);
		REQUIRE(memcmp(&fileContent[token.sourceIndex], "string", 6) == 0);

		REQUIRE(tokenizer.GetToken(token) == compiler::Tokenizer::TokenizerResult::OK);
		REQUIRE(token.lineIndex == 0);
		REQUIRE(token.colIndex == 6);
		REQUIRE(token.sourceIndex == 6);
		REQUIRE(token.sourceLength == 1);
		REQUIRE(token.type == compiler::TokenType::Whitespace);
		REQUIRE(fileContent[token.sourceIndex] == ' ');

		// TODO: Identifier
		REQUIRE(tokenizer.GetToken(token) == compiler::Tokenizer::TokenizerResult::OK);
		REQUIRE(token.lineIndex == 0);
		REQUIRE(token.colIndex == 7);
		REQUIRE(token.sourceIndex == 7);
		REQUIRE(token.sourceLength == 1);
		REQUIRE(token.type == compiler::TokenType::Unknown);
		// REQUIRE(fileContent[token.sourceIndex] == 'p');

		REQUIRE(tokenizer.GetToken(token) == compiler::Tokenizer::TokenizerResult::OK);
		REQUIRE(token.lineIndex == 0);
		REQUIRE(token.colIndex == 8);
		REQUIRE(token.sourceIndex == 8);
		REQUIRE(token.sourceLength == 1);
		REQUIRE(token.type == compiler::TokenType::Whitespace);
		REQUIRE(fileContent[token.sourceIndex] == ' ');

		REQUIRE(tokenizer.GetToken(token) == compiler::Tokenizer::TokenizerResult::OK);
		REQUIRE(token.lineIndex == 0);
		REQUIRE(token.colIndex == 9);
		REQUIRE(token.sourceIndex == 9);
		REQUIRE(token.sourceLength == 1);
		REQUIRE(token.type == compiler::TokenType::Punctuator);
		REQUIRE(fileContent[token.sourceIndex] == '=');

		REQUIRE(tokenizer.GetToken(token) == compiler::Tokenizer::TokenizerResult::OK);
		REQUIRE(token.lineIndex == 0);
		REQUIRE(token.colIndex == 10);
		REQUIRE(token.sourceIndex == 10);
		REQUIRE(token.sourceLength == 1);
		REQUIRE(token.type == compiler::TokenType::Whitespace);
		REQUIRE(fileContent[token.sourceIndex] == ' ');

		REQUIRE(tokenizer.GetToken(token) == compiler::Tokenizer::TokenizerResult::OK);
		REQUIRE(token.lineIndex == 0);
		REQUIRE(token.colIndex == 11);
		REQUIRE(token.sourceIndex == 11);
		REQUIRE(token.sourceLength == 15);
		REQUIRE(token.type == compiler::TokenType::StringLiteral);
		REQUIRE(memcmp(&fileContent[token.sourceIndex], "\"Hello, World!\"", 15) == 0);

		REQUIRE(tokenizer.GetToken(token) == compiler::Tokenizer::TokenizerResult::OK);
		REQUIRE(token.lineIndex == 0);
		REQUIRE(token.colIndex == 26);
		REQUIRE(token.sourceIndex == 26);
		REQUIRE(token.sourceLength == 1);
		REQUIRE(token.type == compiler::TokenType::Punctuator);
		REQUIRE(fileContent[token.sourceIndex] == ';');
	}

	SUBCASE("Char Constant")
	{
		const char* fileContent = "'A';";

		compiler::Token token;
		compiler::Tokenizer tokenizer(fileContent);

		REQUIRE(tokenizer.GetToken(token) == compiler::Tokenizer::TokenizerResult::OK);
		REQUIRE(token.lineIndex == 0);
		REQUIRE(token.colIndex == 0);
		REQUIRE(token.sourceIndex == 0);
		REQUIRE(token.sourceLength == 3);
		REQUIRE(token.type == compiler::TokenType::CharConstant);
		REQUIRE(memcmp(&fileContent[token.sourceIndex], "'A'", 3) == 0);
	}
}




/*
while (true)
{
	compiler::Token token;
	compiler::Tokenizer::TokenizerResult result = tokenizer.GetToken(token);

	if (result == compiler::Tokenizer::TokenizerResult::EndOfSource)
		break;

	const uint32 stringLength = token.sourceLength + 1;

	char* subContent = new char[stringLength];
	memset(subContent, 0, stringLength);
	memcpy(subContent, &fileContent[token.sourceIndex], token.sourceLength);

	std::cout << "Token [" << subContent << "][" << TokenTypeToString(token.type) << "]" << std::endl;

	delete[] subContent;
}*/

//