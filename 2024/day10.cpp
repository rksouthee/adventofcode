#include "aoc.h"
#include "aoc/vector.h"

#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

using aoc::Vector2;

namespace
{
	using Grid = std::vector<std::string>;

	Grid read_input(std::istream& is)
	{
		Grid grid;
		std::string line;
		while (std::getline(is, line))
		{
			grid.push_back(line);
		}
		return grid;
	}

	std::vector<Vector2> get_starting_positions(const Grid& grid)
	{
		std::vector<Vector2> positions;
		for (S64 y = 0; y < std::ssize(grid); ++y)
		{
			const std::string& row = grid[y];
			for (S64 x = 0; x < std::ssize(row); ++x)
			{
				if (row[x] == '0')
				{
					positions.emplace_back(x, y);
				}
			}
		}
		return positions;
	}

	bool in_bounds(const Grid& grid, const Vector2 position)
	{
		return position.x >= 0 && position.y >= 0 && position.x < std::ssize(grid[0]) && position.y < std::ssize(grid);
	}

	bool can_move_to(const Grid& grid, const Vector2 position, const Vector2 direction)
	{
		const Vector2 target = position + direction;
		if (!in_bounds(grid, target))
		{
			return false;
		}

		return grid[target.y][target.x] - grid[position.y][position.x] == 1;
	}

	S64 count_paths(const Grid& grid, const Vector2 position, std::unordered_set<S64>* visited)
	{
		if (visited)
		{
			const S64 p = position.x + position.y * std::ssize(grid[0]);
			if (visited->contains(p))
			{
				return 0;
			}
			visited->insert(p);
		}

		if (grid[position.y][position.x] == '9')
		{
			return 1;
		}

		S64 result = 0;
		if (can_move_to(grid, position, Vector2::east))
		{
			result += count_paths(grid, position + Vector2::east, visited);
		}

		if (can_move_to(grid, position, Vector2::south))
		{
			result += count_paths(grid, position + Vector2::south, visited);
		}

		if (can_move_to(grid, position, Vector2::west))
		{
			result += count_paths(grid, position + Vector2::west, visited);
		}

		if (can_move_to(grid, position, Vector2::north))
		{
			result += count_paths(grid, position + Vector2::north, visited);
		}

		return result;
	}

	S64 get_trailhead_score(const Grid& grid, const Vector2 position)
	{
		std::unordered_set<S64> visited;
		return count_paths(grid, position, &visited);
	}

	S64 part_one(const Grid& grid, const std::vector<Vector2>& starting_positions)
	{
		S64 result = 0;
		for (const Vector2& position : starting_positions)
		{
			result += get_trailhead_score(grid, position);
		}
		return result;
	}

	S64 get_trailhead_rating(const Grid& grid, const Vector2 position)
	{
		return count_paths(grid, position, nullptr);
	}

	S64 part_two(const Grid& grid, const std::vector<Vector2>& starting_positions)
	{
		S64 result = 0;
		for (const Vector2& position : starting_positions)
		{
			result += get_trailhead_rating(grid, position);
		}
		return result;
	}
}

SOLVE
{
	const Grid grid = read_input(std::cin);
	const std::vector<Vector2> starting_positions = get_starting_positions(grid);

	std::cout << part_one(grid, starting_positions) << '\n';
	std::cout << part_two(grid, starting_positions) << '\n';
}