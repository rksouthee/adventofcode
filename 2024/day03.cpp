#include "aoc.h"

#include <iostream>
#include <regex>
#include <string>

namespace
{
	std::vector<std::string> read_input(std::istream& input)
	{
		std::vector<std::string> result;
		std::copy(std::istream_iterator<std::string>(input), std::istream_iterator<std::string>(), std::back_inserter(result));
		return result;
	}

	int part_one(const std::vector<std::string>& input)
	{
		const std::sregex_iterator end;
		const std::regex mul_re("mul\\((\\d{1,3}),(\\d{1,3})\\)");
		int result = 0;
		for (const std::string& memory : input)
		{
			for (auto iter = std::sregex_iterator(memory.begin(), memory.end(), mul_re); iter != end; ++iter)
			{
				std::smatch match = *iter;
				const int a = std::stoi(match[1]);
				const int b = std::stoi(match[2]);
				result += a * b;
			}
		}
		return result;
	}

	int part_two(const std::vector<std::string>& input)
	{
		const std::sregex_iterator end;
		const std::regex re("do\\(\\)|don't\\(\\)|mul\\((\\d{1,3}),(\\d{1,3})\\)");
		int result = 0;
		bool enabled = true;
		for (const std::string& memory : input)
		{
			for (auto iter = std::sregex_iterator(memory.begin(), memory.end(), re); iter != end; ++iter)
			{
				std::smatch match = *iter;
				if (match.str() == "do()")
				{
					enabled = true;
				}
				else if (match.str() == "don't()")
				{
					enabled = false;
				}
				else if (enabled)
				{
					const int a = std::stoi(match[1]);
					const int b = std::stoi(match[2]);
					result += a * b;
				}
			}
		}
		return result;
	}
}

SOLVE
{
	const std::vector<std::string> input = read_input(std::cin);
	std::cout << part_one(input) << std::endl;
	std::cout << part_two(input) << std::endl;
}
