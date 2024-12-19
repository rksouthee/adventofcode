#include "aoc.h"
#include "aoc/core.h"
#include "aoc/string_helpers.h"

#include <algorithm>
#include <iostream>
#include <span>
#include <string>
#include <unordered_map>
#include <vector>

namespace
{
	std::vector<std::string> read_towels(std::istream& in)
	{
		std::string line;
		std::getline(in, line);
		const std::vector<std::string_view> parts = aoc::split(line, ", ");
		std::vector<std::string> towels;
		towels.reserve(parts.size());
		std::transform(parts.begin(), parts.end(), std::back_inserter(towels), [](const std::string_view part)
			{ return std::string(part.begin(), part.end()); });
		return towels;
	}

	std::vector<std::string> read_designs(std::istream& in)
	{
		std::vector<std::string> designs;
		for (std::string line; std::getline(in, line);)
		{
			if (line.empty()) continue;
			designs.push_back(line);
		}
		return designs;
	}

	S64 count_combinations(const std::string_view design, const std::span<const std::string> towels)
	{
		static std::unordered_map<std::string_view, S64> cache;
		if (cache.contains(design))
		{
			return cache[design];
		}

		S64& value = cache[design];
		if (design.empty())
		{
			return value += 1;
		}

		for (const std::string& towel : towels)
		{
			if (design.starts_with(towel))
			{
				value += count_combinations(design.substr(towel.size()), towels);
			}
		}
		return value;
	}
}

SOLVE
{
	const std::vector<std::string> towels = read_towels(std::cin);
	const std::vector<std::string> designs = read_designs(std::cin);

	S64 part_one = 0;
	S64 part_two = 0;

	for (const std::string& design : designs)
	{
		const S64 combinations = count_combinations(design, towels);
		if (combinations) ++part_one;
		part_two += combinations;
	}

	std::cout << part_one << '\n';
	std::cout << part_two << '\n';
}