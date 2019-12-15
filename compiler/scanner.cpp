#include "shared/types.h"
#include "scanner.h"
#include <iostream>

using namespace compiler;

Scanner::ScannerResult Scanner::GetCharacter(Character& character)
{
	const char& c = m_src[m_sourceIndex];

	character.character = c;
	character.col = m_colIndex;
	character.line = m_lineIndex;
	character.sourceIndex = m_sourceIndex;

	if (c == '\0')
	{
		character.type = CharacterType::EndOfFile;

		return ScannerResult::EndOfSource;
	}
	
	// DOS & CP/M EOF: "^Z".
	if (c == 26)
	{
		character.type = CharacterType::EndOfFile;

		return ScannerResult::EndOfSource;
	}

	if (c == '\f' ||
		c == '\v')
	{
		// We're skipping this, ew.
		++m_sourceIndex;
		++m_colIndex;

		return GetCharacter(character);
	}

	// LLVM just treats '\r' as newlines, and you know, I can't argue.
	if (c == '\r' || c == '\n')
	{
		m_colIndex = 0;

		++m_lineIndex;

		character.type = CharacterType::NewLine;
	}
	else
	{
		if (c == '\t')
		{
			character.type = CharacterType::Tab;
		}
		else if (c == ' ')
		{
			character.type = CharacterType::Space;
		}
		else
		{
			character.type = CharacterType::General;
		}

		++m_colIndex;
	}

	++m_sourceIndex;

	return ScannerResult::OK;
}