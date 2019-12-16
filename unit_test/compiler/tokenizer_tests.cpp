#include "doctest/doctest.h"
#include "shared/types.h"
#include "compiler/token.h"
#include <string>
#include <fstream>
#include <streambuf>
#include <iostream>

const std::string hello_world = R"(
// import works like python, it's just a file yo
// Unsure if I want to support full OOP, sounds like a pain
import debug;

int32 main()
{
    debug.print("Hello, World!\n");
})";

static std::string ScannerCharacterTypeToString(compiler::CharacterType& type)
{
    switch (type)
    {
    case compiler::CharacterType::General:
        return "General";
    case compiler::CharacterType::Space:
        return "Space";
    case compiler::CharacterType::Tab:
        return "Tab";
    case compiler::CharacterType::NewLine:
        return "NewLine";
    case compiler::CharacterType::EndOfFile:
        return "EndOfFile";
    }

    return "Unknown";
}

TEST_CASE("Tokenizer Tests")
{
    SUBCASE("Example 1")
    {
        compiler::Tokenizer tok(hello_world.c_str());

        uint32 count = 0;

        while (true)
        {
            compiler::Character c;
            compiler::Tokenizer::Result result = tok.GetCharacter(c);

            if (result == compiler::Tokenizer::Result::EndOfSource)
                break;

            ++count;
        }

        REQUIRE(count == 177);
    }
}