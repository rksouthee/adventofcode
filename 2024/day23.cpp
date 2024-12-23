#include "aoc.h"
#include "aoc/core.h"

#include <catch2/catch_test_macros.hpp>

#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <vector>

namespace
{
	constexpr S64 num_letters = 26;
	constexpr S64 num_nodes = num_letters * num_letters;

	std::vector<std::pair<std::string, std::string>> read_input(std::istream& input)
	{
		std::vector<std::pair<std::string, std::string>> result;
		for (std::string line; std::getline(input, line);)
		{
			result.emplace_back(line.substr(0, 2), line.substr(3));
		}
		return result;
	}

	S64 to_index(const std::string_view node)
	{
		return (node[0] - 'a') * num_letters + (node[1] - 'a');
	}

	std::string to_string(const S64 index)
	{
		std::string result(2, 'a');
		result[0] = static_cast<char>((index / num_letters) + 'a');
		result[1] = static_cast<char>((index % num_letters) + 'a');
		return result;
	}

	using Adjacency_matrix = std::vector<std::vector<int>>;

	Adjacency_matrix adjacency_matrix(const std::vector<std::pair<std::string, std::string>>& input)
	{
		Adjacency_matrix result(num_nodes, std::vector(num_nodes, 0));
		for (const auto& [first, second] : input)
		{
			result[to_index(first)][to_index(second)] = 1;
			result[to_index(second)][to_index(first)] = 1;
		}
		return result;
	}

	S64 part_one(const Adjacency_matrix& matrix)
	{
		S64 result = 0;
		for (S64 a = 0; a < num_nodes; ++a)
		{
			for (S64 b = a + 1; b < num_nodes; ++b)
			{
				if (matrix[a][b] == 0)
				{
					continue;
				}

				for (S64 c = b + 1; c < num_nodes; ++c)
				{
					if (matrix[a][c] == 0 || matrix[b][c] == 0)
					{
						continue;
					}

					if (to_string(a)[0] == 't' || to_string(b)[0] == 't' || to_string(c)[0] == 't')
					{
						++result;
					}
				}
			}
		}
		return result;
	}

	bool is_in_clique(const Adjacency_matrix& matrix, const std::set<S64>& clique, const S64 node)
	{
		return std::ranges::all_of(clique, [node, &matrix](const S64 n) { return matrix[n][node] == 1; });
	}

	std::set<S64> get_clique(const Adjacency_matrix& matrix, const S64 node)
	{
		std::set<S64> clique;
		clique.insert(node);
		for (S64 i = 0; i < num_nodes; ++i)
		{
			if (i == node) continue;
			if (is_in_clique(matrix, clique, i)) clique.insert(i);
		}
		return clique;
	}

	std::string part_two(const Adjacency_matrix& matrix)
	{
		std::set<S64> max_clique;
		for (S64 i = 0; i < num_nodes; ++i)
		{
			if (std::set<S64> clique = get_clique(matrix, i); clique.size() > max_clique.size())
			{
				max_clique = std::move(clique);
			}
		}

		std::string result;
		for (const S64 n : max_clique)
		{
			if (result.empty()) result = to_string(n);
			else result += "," + to_string(n);
		}
		return result;
	}
}

TEST_CASE("my test", "[foo]")
{
	REQUIRE(to_index("tc") == 496);
	REQUIRE(to_string(496) == "tc");
}

SOLVE
{
	const std::vector<std::pair<std::string, std::string>> input = read_input(std::cin);
	const Adjacency_matrix matrix = adjacency_matrix(input);
	std::cout << part_one(matrix) << '\n';
	std::cout << part_two(matrix) << '\n';
}