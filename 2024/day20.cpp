#include "aoc.h"
#include "aoc/vector.h"

#include <array>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using aoc::Vector2;

namespace
{
	using Grid = std::vector<std::string>;

	Grid read_input(std::istream& is)
	{
		Grid grid;
		for (std::string line; std::getline(is, line);)
		{
			grid.push_back(line);
		}
		return grid;
	}

	Vector2 find_position(const Grid& grid, const char ch)
	{
		for (S64 y = 0; y < std::ssize(grid); ++y)
		{
			for (S64 x = 0; x < std::ssize(grid); ++x)
			{
				if (grid[y][x] == ch)
				{
					return { x, y };
				}
			}
		}
		return { -1, -1 };
	}

	bool is_empty(const Grid& grid, const Vector2& position)
	{
		if (position.x < 0 || position.x >= std::ssize(grid[0]) || position.y < 0 || position.y >= std::ssize(grid))
		{
			return false;
		}
		return grid[position.y][position.x] != '#';
	}

	std::vector<Vector2> get_shortest_path(const Grid& grid, const Vector2& start, const Vector2& end)
	{
		std::vector path = { start };
		while (true)
		{
			Vector2 current = path.back();
			if (current == end) break;
			const std::array directions = { Vector2::east, Vector2::south, Vector2::west, Vector2::north };
			for (const Vector2 direction : directions)
			{
				const Vector2 target = current + direction;
				if (is_empty(grid, target) && (path.size() < 2 || target != path[path.size() - 2]))
				{
					path.push_back(target);
					break;
				}
			}
		}
		return path;
	}

	S64 count_cheats(const std::vector<Vector2>& path, const S64 picoseconds, const S64 saved)
	{
		const S64 path_len = std::ssize(path) - 1;
		S64 count = 0;
		for (S64 i = 0; i < path_len; ++i)
		{
			for (S64 j = i + saved + 1; j <= path_len; ++j)
			{
				if (const S64 distance = manhattan_distance(path[i], path[j]); distance <= picoseconds)
				{
					if (j - i - distance >= saved) ++count;
				}
			}
		}
		return count;
	}

	S64 part_one(const std::vector<Vector2>& path)
	{
		return count_cheats(path, 2, 100);
	}

	S64 part_two(const std::vector<Vector2>& path)
	{
		return count_cheats(path, 20, 100);
	}
}

SOLVE
{
	const Grid grid = read_input(std::cin);
	const Vector2 start = find_position(grid, 'S');
	const Vector2 end = find_position(grid, 'E');
	const std::vector<Vector2> path = get_shortest_path(grid, start, end);

	std::cout << part_one(path) << '\n';
	std::cout << part_two(path) << '\n';
}