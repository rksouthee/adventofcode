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

	int multiply_match(const std::smatch& match)
	{
		return std::stoi(match[1]) * std::stoi(match[2]);
	}

	int part_one(const std::vector<std::string>& input)
	{
		const std::regex mul_re(R"(mul\((\d{1,3}),(\d{1,3})\))");
		int result = 0;
		for (const std::string& memory : input)
		{
			const std::sregex_iterator end;
			for (auto iter = std::sregex_iterator(memory.begin(), memory.end(), mul_re); iter != end; ++iter)
			{
				result += multiply_match(*iter);
			}
		}
		return result;
	}

	int part_two(const std::vector<std::string>& input)
	{
		const std::regex re(R"(do\(\)|don't\(\)|mul\((\d{1,3}),(\d{1,3})\))");
		int result = 0;
		bool enabled = true;
		for (const std::string& memory : input)
		{
			const std::sregex_iterator end;
			for (auto iter = std::sregex_iterator(memory.begin(), memory.end(), re); iter != end; ++iter)
			{
				if (iter->str() == "do()")
				{
					enabled = true;
				}
				else if (iter->str() == "don't()")
				{
					enabled = false;
				}
				else if (enabled)
				{
					result += multiply_match(*iter);
				}
			}
		}
		return result;
	}
}

SOLVE
{
	const std::vector<std::string> input = read_input(std::cin);
	std::cout << part_one(input) << '\n';
	std::cout << part_two(input) << '\n';
}
