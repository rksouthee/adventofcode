#include "aoc.h"
#include "aoc/core.h"
#include "aoc/vector.h"

#include <iostream>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using aoc::Vector2;

namespace
{
	using Grid = std::vector<std::string>;

	Grid read_input(std::istream& input)
	{
		Grid grid;
		std::string line;
		while (std::getline(input, line))
		{
			grid.push_back(line);
		}
		return grid;
	}

	std::unordered_map<char, std::vector<Vector2>> group_antennas(const Grid& grid)
	{
		std::unordered_map<char, std::vector<Vector2>> antennas;
		for (S64 y = 0; y < std::ssize(grid); ++y)
		{
			const std::string& row = grid[y];
			for (S64 x = 0; x < std::ssize(row); ++x)
			{
				if (const char c = row[x]; c != '.')
				{
					antennas[c].emplace_back(x, y);
				}
			}
		}
		return antennas;
	}

	void add_antinodes_p1(const Grid&, const std::vector<Vector2>& positions, std::vector<Vector2>& antinodes)
	{
		for (std::size_t i = 0; i < positions.size(); ++i)
		{
			for (std::size_t j = i + 1; j < positions.size(); ++j)
			{
				const Vector2 direction = positions[j] - positions[i];
				antinodes.push_back(positions[j] - 2 * direction);
				antinodes.push_back(positions[i] + 2 * direction);
			}
		}
	}

	bool is_in_bounds(const Grid& grid, const Vector2& position)
	{
		return position.x >= 0 && position.y >= 0 && position.y < std::ssize(grid) && position.x < std::ssize(grid[0]);
	}

	S64 count_unique_antennas(const Grid& grid, void (*adder)(const Grid&, const std::vector<Vector2>&, std::vector<Vector2>&))
	{
		const std::unordered_map<char, std::vector<Vector2>> antennas = group_antennas(grid);
		std::vector<Vector2> antinodes;
		for (const auto& pair : antennas)
		{
			adder(grid, pair.second, antinodes);
		}

		std::unordered_set<S64> positions;
		for (const Vector2& position : antinodes)
		{
			if (is_in_bounds(grid, position))
			{
				positions.insert(position.y * std::ssize(grid[0]) + position.x);
			}
		}

		return std::ssize(positions);
	}

	S64 part_one(const Grid& grid)
	{
		return count_unique_antennas(grid, add_antinodes_p1);
	}

	void add_while_in_bounds(const Grid& grid, const Vector2& position, const Vector2& direction, std::vector<Vector2>& antinodes)
	{
		Vector2 current = position + direction;
		while (is_in_bounds(grid, current))
		{
			antinodes.push_back(current);
			current += direction;
		}
	}

	void add_antinodes_p2(const Grid& grid, const std::vector<Vector2>& positions, std::vector<Vector2>& antinodes)
	{
		for (std::size_t i = 0; i < positions.size(); ++i)
		{
			for (std::size_t j = i + 1; j < positions.size(); ++j)
			{
				const Vector2 direction = positions[j] - positions[i];
				add_while_in_bounds(grid, positions[i], direction, antinodes);
				add_while_in_bounds(grid, positions[i], -direction, antinodes);
				add_while_in_bounds(grid, positions[j], direction, antinodes);
				add_while_in_bounds(grid, positions[j], -direction, antinodes);
			}
		}
	}

	S64 part_two(const Grid& grid)
	{
		return count_unique_antennas(grid, add_antinodes_p2);
	}
}

SOLVE
{
	const Grid grid = read_input(std::cin);
	std::cout << part_one(grid) << '\n';
	std::cout << part_two(grid) << '\n';
}
