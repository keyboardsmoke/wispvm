#include "shared/types.h"
#include "context.h"
#include "register.h"
#include "string_formatter.h"
#include <iostream>
#include <regex>
#include <sstream>
#include <cassert>

using namespace wisp;

// TODO: Replace with a function that calls a callback on matching, to replace the text in the result
// We can make a lambda to decide what to do when we run into our matches
// It'll be a lot cleaner, and automatic. Right now it's a bit of a mess.

// TODO v2: Additional operations for a match, i.e. {0:X}, {1:FP6} (fp precision), so on.

bool GetMatches(const std::string str, std::function<void(std::smatch&)> cb)
{
	bool anyMatches = false;

	std::regex r("\\{\\d+\\}");

	for (auto it = std::sregex_iterator(str.begin(), str.end(), r); it != std::sregex_iterator(); it++)
	{
		if (!anyMatches)
			anyMatches = true;

		std::smatch match = *it;

		cb(match);
	}

	return anyMatches;
}

std::string ValueToString(Value& value)
{
	std::string str;

	if (value.IsIntegerValue())
	{
		std::visit([&str](auto&& arg)
		{
			std::stringstream tmp;
			tmp << arg;
			str = tmp.str();
		},
		value.Get<IntegerValue>().GetValue());
	}
	else if (value.IsFPValue())
	{
		std::visit([&str](auto&& arg)
		{
			std::stringstream tmp;
			tmp << arg;
			str = tmp.str();
		},
		value.Get<FPValue>().GetValue());
	}
	else
	{
		str = "<ERROR>";
	}

	return str;
}

std::string StringFormatter::GetFormattedString(WispContext* context)
{
	size_t pos = 0;

	std::string formatted;

	if (GetMatches(m_fmt, [this, context, &pos, &formatted](std::smatch& match)
	{
		formatted.append(m_fmt.substr(pos, match.position() - pos));

		uint32 registerNumber = 0;
		std::stringstream ss(match[0].str().substr(1, match[0].str().length() - 1));
		ss >> registerNumber;

		// R0 is reserved for the format string
		++registerNumber;

		assert(registerNumber < static_cast<uint32>(GeneralPurposeRegisters::Count));

		Register& reg = context->regGp[registerNumber];

		std::string valueString = ValueToString(reg.GetValue());

		formatted.append(valueString);

		pos = match.position() + match.length();
	}))
	{
		if (pos < m_fmt.length())
		{
			formatted.append(m_fmt.substr(pos));
		}

		return formatted;
	}

	return m_fmt;
}