#include "doctest/doctest.h"
#include "shared/types.h"
#include "compiler/scanner.h"
#include <string>
#include <fstream>
#include <streambuf>
#include <iostream>

static std::string ReadEntireFile(const std::string& filename)
{
    std::ifstream ifs(filename, std::ios::binary);
    return std::string((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
}

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

TEST_CASE("Scanner Tests")
{
    SUBCASE("Example 1")
    {
        std::string fileContent = ReadEntireFile("C:\\Users\\kbd\\ClionProjects\\WispVM\\examples\\hello_world.wisp");

        compiler::Scanner scanner(fileContent.c_str());

        uint32 count = 0;

        while (true)
        {
            compiler::Character c;
            compiler::Scanner::ScannerResult result = scanner.GetCharacter(c);

            if (result == compiler::Scanner::ScannerResult::EndOfSource)
                break;

            ++count;
        }

        REQUIRE(count == 72);
    }
}