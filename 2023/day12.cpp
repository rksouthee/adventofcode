#include "aoc.h"
#include <catch2/catch_test_macros.hpp>

#include <algorithm>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

#include <cassert>
#include <cstddef>

namespace
{
	struct Row
	{
		std::string springs;
		std::vector<std::uint64_t> groups;
	};

	std::vector<Row> read_input(std::istream& is)
	{
		std::vector<Row> rows;
		for (std::string line; std::getline(is, line);)
		{
			char c;
			std::istringstream iss(line);
			auto& row = rows.emplace_back();
			iss >> row.springs;
			do iss >> row.groups.emplace_back() >> c;
			while (iss);
		}
		return rows;
	}

	using Table = std::vector<std::vector<std::vector<std::uint64_t>>>;

	std::uint64_t*** make_table(const std::string& str, const std::vector<std::uint64_t>& groups)
	{
		std::uint64_t*** table = new std::uint64_t**[str.size()];
		for (std::size_t i = 0; i < str.size(); ++i)
		{
			table[i] = new std::uint64_t*[groups.size()];
			for (std::size_t j = 0; j < groups.size(); ++j)
			{
				table[i][j] = new std::uint64_t[groups[j] + 1]{};
			}
		}

		switch (str.front())
		{
		case '#': table[0][0][1] = 1; break;
		case '.': table[0][0][0] = 1; break;
		case '?': table[0][0][0] = 1;
			  table[0][0][1] = 1; break;
		}

		return table;
	}

	std::uint64_t count_solutions(const std::string& str, const std::vector<std::uint64_t>& groups)
	{
		std::uint64_t*** table = make_table(str, groups);
		std::ptrdiff_t pound_count = 0;

		for (std::size_t i = 1; i < str.size(); ++i)
		{
			if (str[i - 1] == '#') ++pound_count;
			const char ch = str[i];
			for (std::size_t j = 0; j < groups.size(); ++j)
			{
				const std::uint64_t group = groups[j];
				for (std::uint64_t k = 0; k <= group; ++k)
				{
					if (ch == '.' || ch == '?')
					{
						if (k == 0)
						{
							if (j > 0)
							{
								table[i][j][k] += table[i - 1][j - 1][groups[j - 1]] + table[i - 1][j][0];
							}
							else if (pound_count == 0)
							{
								table[i][j][k] = 1;
							}
						}
					}

					if (ch == '#' || ch == '?')
					{
						if (k > 0) table[i][j][k] += table[i - 1][j][k - 1];
					}
				}
			}
		}
		const std::uint64_t result = table[str.size() - 1][groups.size() - 1][0];
		for (std::size_t i = 0; i < str.size(); ++i)
		{
			for (std::size_t j = 0; j < groups.size(); ++j)
			{
				delete[] table[i][j];
			}
			delete[] table[i];
		}
		delete[] table;
		return result;
	}

	std::uint64_t solve(const std::string& str, std::vector<std::uint64_t> groups)
	{
		groups.push_back(0);
		return count_solutions(str + '.', groups);
	}

	Row unfold(const Row& row, std::ptrdiff_t n)
	{
		std::string springs = row.springs;;
		for (std::ptrdiff_t i = 1; i < n; ++i)
		{
			springs += '?' + row.springs;
		}
		std::vector<std::uint64_t> groups;
		for (std::ptrdiff_t i = 0; i < n; ++i)
		{
			groups.insert(groups.end(), row.groups.begin(), row.groups.end());
		}
		return { std::move(springs), std::move(groups) };
	}

	std::uint64_t part_one(const std::vector<Row>& rows)
	{
		std::uint64_t sum = 0;
		for (const Row& row : rows) sum += solve(row.springs, row.groups);
		return sum;
	}

	std::uint64_t part_two(const std::vector<Row>& rows)
	{
		std::uint64_t sum = 0;
		for (const Row& row : rows)
		{
			Row row_unfolded = unfold(row, 5);
			sum += solve(row_unfolded.springs, row_unfolded.groups);
		}
		return sum;
	}
}

TEST_CASE("count solutions", "[count]")
{
	REQUIRE(solve("#", {1}) == 1);
	REQUIRE(solve(".", {1}) == 0);
	REQUIRE(solve("?", {1}) == 1);
	REQUIRE(solve("???", {1, 1}) == 1);
	REQUIRE(solve("????", {2, 1}) == 1);
	REQUIRE(solve("##..#", {2, 1}) == 1);
	REQUIRE(solve("?????", {2, 1}) == 3);
	REQUIRE(solve("???.###", {1,1,3}) == 1);
	REQUIRE(solve(".??..??...?##.", {1,1,3}) == 4);
	REQUIRE(solve("?#?#?#?#?#?#?#?", {1,3,1,6}) == 1);
	REQUIRE(solve("????.#...#...", {4,1,1}) == 1);
	REQUIRE(solve("????.######..#####.", {1,6,5}) == 4);
	REQUIRE(solve("?###????????", {3,2,1}) == 10);
}

SOLVE
{
	const std::vector<Row> rows = read_input(std::cin);

	std::cout << part_one(rows) << std::endl;
	std::cout << part_two(rows) << std::endl;
}
