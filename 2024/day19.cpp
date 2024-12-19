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

	bool is_possible(const std::string_view design, const std::span<const std::string> towels)
	{
		if (design.empty()) return true;
		for (const std::string& towel : towels)
		{
			if (design.starts_with(towel) && is_possible(design.substr(towel.size()), towels))
			{
				return true;
			}
		}
		return false;
	}

	bool is_possible(const std::string_view design, const std::unordered_map<char, std::span<const std::string>>& towels)
	{
		if (design.empty()) return true;
		if (!towels.contains(design[0])) return false;
		for (const std::string& towel : towels.at(design[0]))
		{
			if (design.starts_with(towel) && is_possible(design.substr(towel.size()), towels))
			{
				return true;
			}
		}
		return false;
	}

	std::vector<std::string> unique_towels(std::vector<std::string> towels)
	{
		std::vector<std::string> result;
		for (S64 i = 0; i < std::ssize(towels); ++i)
		{
			std::swap(towels[0], towels[i]);
			if (!is_possible(towels[0], std::span(towels).subspan(1)))
			{
				result.push_back(towels[0]);
			}
		}
		std::ranges::sort(result);
		return result;
	}

	std::unordered_map<char, std::span<const std::string>> create_map(const std::vector<std::string>& towels)
	{
		S64 start = 0;
		char current = towels[0][0];
		std::unordered_map<char, std::span<const std::string>> result;
		for (S64 i = 1; i < std::ssize(towels); ++i)
		{
			if (towels[i][0] != current)
			{
				result[current] = std::span(towels).subspan(start, i - start);
				start = i;
				current = towels[i][0];
			}
		}
		result[current] = std::span(towels).subspan(start);
		return result;
	}

	S64 part_one(const std::unordered_map<char, std::span<const std::string>>& towels, const std::vector<std::string>& designs)
	{
		return std::ranges::count_if(designs, [&towels](const std::string& design) -> bool { return is_possible(design, towels); });
	}
}

SOLVE
{
	const std::vector<std::string> towels = read_towels(std::cin);
	const std::vector<std::string> designs = read_designs(std::cin);
	const std::vector<std::string> unique = unique_towels(towels);
	const std::unordered_map<char, std::span<const std::string>> map = create_map(unique);

	std::cout << part_one(map, designs) << '\n';
}