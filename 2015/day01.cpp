#include "aoc.h"
#include "aoc/core.h"

#include <iostream>
#include <string>

namespace
{
	std::string read_input(std::istream& is)
	{
		std::string result;
		is >> result;
		return result;
	}

	S64 map_char(const char ch)
	{
		if (ch == '(')
		{
			return 1;
		}
		return -1;
	}

	S64 part_one(const std::string_view input)
	{
		S64 result = 0;
		for (const char ch : input)
		{
			result += map_char(ch);
		}
		return result;
	}

	S64 part_two(const std::string_view input)
	{
		S64 floor = 0;
		for (S64 i = 0; i < std::ssize(input); ++i)
		{
			floor += map_char(input[i]);
			if (floor == -1)
			{
				return i + 1;
			}
		}
		return -1;
	}
}

SOLVE
{
	const std::string input = read_input(std::cin);

	std::cout << part_one(input) << '\n';
	std::cout << part_two(input) << '\n';
}