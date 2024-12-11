#include "aoc.h"
#include "aoc/core.h"
#include "aoc/string_helpers.h"

#include <iostream>
#include <map>
#include <numeric>
#include <string>
#include <vector>

namespace
{
	std::vector<S64> read_input(std::istream& input)
	{
		using I = std::istream_iterator<S64>;
		return { I(input), I() };
	}

	std::vector<S64> transform(const S64 value)
	{
		std::vector<S64> result;
		if (value == 0)
		{
			result.push_back(1);
		}
		else if (const std::string str = std::to_string(value); str.size() % 2 == 0)
		{
			const U64 half = str.size() / 2;
			result.push_back(aoc::convert_unguarded<S64>(str.substr(0, half)));
			result.push_back(aoc::convert_unguarded<S64>(str.substr(half)));
		}
		else
		{
			result.push_back(value * 2024);
		}
		return result;
	}

	S64 blink(const S64 value, const int steps)
	{
		static std::map<std::pair<S64, int>, S64> s_cache;
		S64& result = s_cache[{value, steps}];
		if (result != 0)
		{
			return result;
		}

		const std::vector<S64> input = transform(value);
		if (steps == 1)
		{
			result = std::ssize(input);
			return result;
		}

		for (const S64 next : input)
		{
			result += blink(next, steps - 1);
		}

		return result;
	}

	S64 count_stones(const std::vector<S64>& input, const int steps)
	{
		S64 result = 0;
		for (const S64 value : input)
		{
			result += blink(value, steps);
		}
		return result;
	}

	S64 part_one(const std::vector<S64>& input)
	{
		return count_stones(input, 25);
	}

	S64 part_two(const std::vector<S64>& input)
	{
		return count_stones(input, 75);
	}
}

SOLVE
{
	const std::vector<S64> input = read_input(std::cin);

	std::cout << part_one(input) << '\n';
	std::cout << part_two(input) << '\n';
}