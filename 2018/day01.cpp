#include <catch2/catch_test_macros.hpp>

#include <iostream>
#include <iterator>
#include <numeric>
#include <unordered_set>
#include <vector>

namespace
{
	int part_one(const std::vector<int>& changes)
	{
		return std::accumulate(changes.begin(), changes.end(), 0);
	}

	int part_two(const std::vector<int>& changes)
	{
		std::unordered_set<int> seen;
		int frequency = 0;
		std::size_t index = 0;
		while (!seen.contains(frequency))
		{
			seen.insert(frequency);
			if (index == changes.size())
			{
				index = 0;
			}
			frequency += changes[index];
			++index;
		}
		return frequency;
	}
}

TEST_CASE("part one", "[one]")
{
	REQUIRE(part_one({1, -2, 3, 1}) == 3);
	REQUIRE(part_one({1, 1, 1}) == 3);
	REQUIRE(part_one({1, 1, -2}) == 0);
	REQUIRE(part_one({-1, -2, -3}) == -6);
}

TEST_CASE("part two", "[two]")
{
	REQUIRE(part_two({1, -2, 3, 1}) == 2);
	REQUIRE(part_two({1, -1}) == 0);
	REQUIRE(part_two({3, 3, 4, -2, -4}) == 10);
	REQUIRE(part_two({-6, 3, 8, 5, -6}) == 5);
	REQUIRE(part_two({7, 7, -2, -7, -4}) == 14);
}

extern "C" void solve(std::istream& is)
{
	using I = std::istream_iterator<int>;
	const std::vector<int> changes(I{is}, I{});
	
	std::cout << part_one(changes) << std::endl;
	std::cout << part_two(changes) << std::endl;
}
