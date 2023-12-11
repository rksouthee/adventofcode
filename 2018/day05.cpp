#include "aoc.h"

#include <catch2/catch_test_macros.hpp>

#include <iostream>
#include <string>

namespace
{
	std::string read_input(std::istream& is)
	{
		std::string input;
		is >> input;
		return input;
	}

	bool reacts(const unsigned x, const unsigned y)
	{
		return (x ^ y) == 0b00100000;
	}

	std::string react(std::string polymer)
	{
		std::size_t i = 1;
		while (i < polymer.size())
		{
			if (i > 0 && reacts(polymer[i - 1], polymer[i]))
			{
				--i;
				polymer.erase(i, 2);
			}
			else
			{
				++i;
			}
		}
		return polymer;
	}

	std::size_t part_one(const std::string& polymer)
	{
		return react(polymer).size();
	}

	std::size_t part_two(const std::string& polymer)
	{
		auto shortest = std::numeric_limits<std::size_t>::max();
		for (char c = 'a'; c <= 'z'; ++c)
		{
			std::string p = polymer;
			const auto iter = std::remove_if(p.begin(), p.end(), [c] (const char ch) -> bool { return std::tolower(ch) == c; });
			p.erase(iter, p.end());
			p = react(p);
			shortest = std::min(shortest, p.size());
		}
		return shortest;
	}
}

TEST_CASE("polymer reactions", "[part one]")
{
	REQUIRE(react("aA") == "");
	REQUIRE(react("abBA") == "");
	REQUIRE(react("abAB") == "abAB");
	REQUIRE(react("aabAAB") == "aabAAB");
}

SOLVE
{
	const std::string input = read_input(std::cin);

	std::cout << part_one(input) << std::endl;
	std::cout << part_two(input) << std::endl;
}
