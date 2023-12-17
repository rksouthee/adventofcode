#include "aoc.h"

#include <catch2/catch_test_macros.hpp>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

#include <cassert>
#include <cstdint>

namespace
{
	enum class Direction
	{
		right,
		down,
		left,
		up
	};

	struct Node
	{
		std::uint32_t x;
		std::uint32_t y;
		Direction d;
		std::uint32_t k;
			
		operator std::uint32_t() const
		{
			assert(k >= 0 && k < 16);
			return (k << 28) | ((std::uint32_t)d << 26) | (y << 13) | x;
		}

		std::strong_ordering operator<=>(const Node&) const = default;
	};
}

namespace std
{
	template <>
	struct hash<Node>
	{
		std::uint64_t operator()(const Node& node) const
		{
			return std::hash<std::uint32_t>()(static_cast<std::uint32_t>(node));
		}
	};
}

namespace
{
	using Grid = std::vector<std::string>;

	Grid read_input(std::istream& is)
	{
		using I = std::istream_iterator<std::string>;
		return { I{is}, I{} };
	}

	std::vector<std::pair<int, Node>> get_neighbours(const Grid& grid, const Node& node, std::uint32_t min, std::uint32_t max)
	{
		std::vector<std::pair<int, Node>> result;
		// right
		if (node.d != Direction::left && node.d != Direction::right)
		{
			int heat = 0;
			for (std::uint32_t k = 1; k <= max && node.x + k < grid.front().size(); ++k)
			{
				heat += grid[node.y][node.x + k] - '0';
				if (k >= min)
				{
					result.emplace_back(heat, Node{node.x + k, node.y, Direction::right, k});
				}
			}
		}
		// down
		if (node.d != Direction::up && node.d != Direction::down)
		{
			int heat = 0;
			for (std::uint32_t k = 1; k <= max && node.y + k < grid.size(); ++k)
			{
				heat += grid[node.y + k][node.x] - '0';
				if (k >= min)
				{
					result.emplace_back(heat, Node{node.x, node.y + k, Direction::down, k});
				}
			}
		}
		// left
		if (node.d != Direction::right && node.d != Direction::left)
		{
			int heat = 0;
			for (std::uint32_t k = 1; k <= max && node.x > k - 1; ++k)
			{
				heat += grid[node.y][node.x - k] - '0';
				if (k >= min)
				{
					result.emplace_back(heat, Node{node.x - k, node.y, Direction::left, k});
				}
			}
		}
		// up
		if (node.d != Direction::down && node.d != Direction::up)
		{
			int heat = 0;
			for (std::uint32_t k = 1; k <= max && node.y > k - 1; ++k)
			{
				heat += grid[node.y - k][node.x] - '0';
				if (k >= min)
				{
					result.emplace_back(heat, Node{node.x, node.y - k, Direction::up, k});
				}
			}
		}
		return result;
	}

	int shortest_path(const Grid& grid, std::uint32_t min, std::uint32_t max)
	{
		std::unordered_map<Node, int> dist;
		for (std::size_t y = 0; y < grid.size(); ++y)
		{
			for (std::size_t x = 0; x < grid[y].size(); ++x)
			{
				for (std::uint32_t d = 0; d < 4; ++d)
				{
					for (std::uint32_t k = min; k <= max; ++k)
					{
						const Node node{(std::uint32_t)x, (std::uint32_t)y, (Direction)d, k};
						dist[node] = std::numeric_limits<int>::max();
					}
				}
			}
		}

		const auto r = [&dist] (const Node& x, const Node& y) -> bool
		{
			return dist[x] > dist[y];
		};
		std::priority_queue<Node, std::vector<Node>, decltype(r)> queue(r);

		for (const Direction d : { Direction::right, Direction::down} )
		{
			const Node node{0, 0, d, 0};
			dist[node] = 0;
			queue.push(node);
		}

		while (!queue.empty())
		{
			const Node u = queue.top();
			queue.pop();

			for (const std::pair<int, Node>& pair : get_neighbours(grid, u, min, max))
			{
				int alt = dist[u] + pair.first;
				if (alt < dist[pair.second])
				{
					queue.push(pair.second);
					dist[pair.second] = alt;
				}
			}
		}

		auto min_dist = std::numeric_limits<int>::max();
		for (std::uint32_t d = 0; d < 4; ++d)
		{
			for (std::uint32_t k = min; k <= max; ++k)
			{
				const Node node{(std::uint32_t)grid.front().size() - 1, (std::uint32_t)grid.size() - 1, (Direction)d, k};
				min_dist = std::min(min_dist, dist[node]);
			}
		}
		return min_dist;
	}
}

TEST_CASE("get neighbours", "[neighbours]")
{
	std::vector<std::string> grid = { "11111" };
	{
		const std::vector<std::pair<int, Node>> neighbours = get_neighbours(grid, Node{0, 0, Direction::down, 0}, 1, 3);
		REQUIRE(neighbours.size() == 3);
		for (std::size_t i = 0; i < neighbours.size(); ++i)
		{
			REQUIRE(neighbours[i].second == Node{(std::uint32_t)i + 1, 0, Direction::right, (std::uint32_t)(i + 1)});
		}
	}
}

TEST_CASE("shortest paths", "[path]")
{
	std::vector<std::string> grid =
	{
		"1111",
		"1121",
		"1111",
		"1111",
	};
	{
		REQUIRE(shortest_path(grid, 1, 3) == 6);
	}
}

SOLVE
{
	const Grid grid = read_input(std::cin);

	std::cout << shortest_path(grid, 1, 3) << std::endl;
	std::cout << shortest_path(grid, 4, 10) << std::endl;
}
