#include "aoc.h"
#include <aoc/string_helpers.h>

#include <catch2/catch_test_macros.hpp>

#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <cstdint>

namespace
{
	std::uint8_t hash(const std::string_view str)
	{
		std::uint8_t current_value = 0;
		for (const char ch : str)
		{
			current_value = (current_value + (std::uint8_t)ch) * 17;
		}
		return current_value;
	}

	std::string read_input(std::istream& is)
	{
		std::string line;
		std::getline(is, line);
		return line;
	}

	std::uint64_t part_one(const std::vector<std::string_view>& parts)
	{
		std::uint64_t sum = 0;
		for (const std::string_view part : parts) sum += hash(part);
		return sum;
	}

	std::uint64_t part_two(const std::vector<std::string_view>& parts)
	{
		using Pair = std::pair<std::string_view, std::uint64_t>;
		std::array<std::vector<Pair>, 256> boxes{};
		for (const std::string_view part : parts)
		{
			if (part.back() == '-')
			{
				const std::string_view label(part.begin(), part.end() - 1);
				const std::uint8_t h = hash(label);
				std::vector<Pair>& pairs = boxes[h];
				pairs.erase(std::remove_if(pairs.begin(), pairs.end(), [&label] (const Pair& pair) -> bool { return pair.first == label; }), pairs.end());
			}
			else if (part[part.size() - 2] == '=')
			{
				const std::string_view label(part.begin(), part.end() - 2);
				const std::uint8_t h = hash(label);
				std::vector<Pair>& pairs = boxes[h];
				const auto iter = std::find_if(pairs.begin(), pairs.end(), [&label] (const Pair& pair) -> bool { return pair.first == label; });
				if (iter != pairs.end()) iter->second = part.back() - '0';
				else pairs.emplace_back(label, part.back() - '0');
			}
		}

		std::uint64_t sum = 0;
		for (std::size_t i = 0; i < boxes.size(); ++i)
		{
			for (std::size_t j = 0; j < boxes[i].size(); ++j)
			{
				const std::uint64_t power = (i + 1) * (j + 1) * boxes[i][j].second;
				sum += power;
			}
		}
		return sum;
	}
}

TEST_CASE("hash", "[hash]")
{
	REQUIRE(hash("HASH") == 52);
}

SOLVE
{
	const std::string input = read_input(std::cin);
	const std::vector<std::string_view> parts = aoc::split(input, ',');

	std::cout << part_one(parts) << std::endl;
	std::cout << part_two(parts) << std::endl;
}
