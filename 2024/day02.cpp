#include "aoc.h"
#include "aoc/string_helpers.h"

#include <algorithm>
#include <charconv>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

namespace
{
	std::vector<std::vector<int>> read_input(std::istream& input)
	{
		std::vector<std::vector<int>> result;
		std::string line;
		while (std::getline(input, line))
		{
			const auto items = aoc::split(line, ' ');
			auto& row = result.emplace_back();
			std::transform(items.begin(), items.end(), std::back_inserter(row), [](const std::string_view item)
				{
					int value = 0;
					std::from_chars(item.data(), item.data() + item.size(), value);
					return value;
				});
		}
		return result;
	}

	bool is_safe(const std::vector<int>& levels)
	{
		std::vector<int> differences;
		differences.reserve(levels.size());
		std::adjacent_difference(levels.begin(), levels.end(), std::back_inserter(differences));

		if (differences.size() < 2)
		{
			return true;
		}

		const auto first = differences.begin() + 1;
		const auto last = differences.end();

		if (*first < 0)
		{
			return std::all_of(first, last, [](int diff) { return diff < 0 && diff >= -3; });
		}

		return std::all_of(first, last, [](int diff) { return diff > 0 && diff <= 3; });
	}

	std::ptrdiff_t part_one(const std::vector<std::vector<int>>& input)
	{
		return std::count_if(input.begin(), input.end(), is_safe);
	}

	bool try_to_make_safe(std::vector<int> levels, std::size_t index)
	{
		levels.erase(levels.begin() + index);
		return is_safe(levels);
	}

	bool is_safe_with_dampener(const std::vector<int>& levels)
	{
		if (is_safe(levels))
		{
			return true;
		}

		for (std::size_t i = 0; i < levels.size(); ++i)
		{
			if (try_to_make_safe(levels, i))
			{
				return true;
			}
		}

		return false;
	}

	std::ptrdiff_t part_two(const std::vector<std::vector<int>>& input)
	{
		return std::count_if(input.begin(), input.end(), is_safe_with_dampener);
	}
}

SOLVE
{
	const auto input = read_input(std::cin);
	std::cout << part_one(input) << std::endl;
	std::cout << part_two(input) << std::endl;
}