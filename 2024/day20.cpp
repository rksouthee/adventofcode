#include "aoc.h"
#include "aoc/vector.h"

#include <array>
#include <cassert>
#include <iostream>
#include <queue>
#include <map>
#include <set>
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
		std::vector<std::vector<S64>> distance_map(std::ssize(grid), std::vector<S64>(std::ssize(grid), -1));
		std::vector<std::vector<Vector2>> parent_map(std::ssize(grid), std::vector<Vector2>(std::ssize(grid), { -1, -1 }));
		std::queue<Vector2> queue;
		queue.push(start);
		distance_map[start.y][start.x] = 0;

		while (!queue.empty())
		{
			const Vector2 current = queue.front();
			if (current == end)
			{
				break;
			}
			queue.pop();

			const std::array directions = { Vector2::east, Vector2::south, Vector2::west, Vector2::north };
			for (const Vector2 direction : directions)
			{
				const Vector2 target = current + direction;
				if (is_empty(grid, target) && distance_map[target.y][target.x] == -1)
				{
					if (grid[target.y][target.x] == '2' && grid[current.y][current.x] != '1') continue;
					distance_map[target.y][target.x] = distance_map[current.y][current.x] + 1;
					parent_map[target.y][target.x] = current;
					queue.push(target);
				}
			}
		}
		std::vector<Vector2> path = { end };
		Vector2 current = end;
		while (current != start)
		{
			if (current.y < 0 || current.x < 0) return {};
			path.push_back(current);
			current = parent_map[current.y][current.x];
		}
		std::ranges::reverse(path);
		return path;
	}

	bool is_wall(const Grid& grid, const Vector2& position)
	{
		if (position.y < 0 || position.y >= std::ssize(grid) || position.x < 0 || position.x >= std::ssize(grid[0]))
		{
			return false;
		}
		return grid[position.y][position.x] == '#';
	}

	void add_potential_walls_to_remove(const Grid& grid, std::set<std::pair<Vector2, Vector2>>& walls, const Vector2& position)
	{
		const std::array directions = { Vector2::east, Vector2::south, Vector2::west, Vector2::north };
		for (const Vector2 direction : directions)
		{
			const Vector2 current = position + direction;
			if (is_empty(grid, current))
			{
				walls.emplace(position, current);
			}
		}
	}

	std::set<std::pair<Vector2, Vector2>> find_potential_walls(const Grid& grid)
	{
		std::set<std::pair<Vector2, Vector2>> walls;
		for (S64 y = 0; y < std::ssize(grid); ++y)
		{
			for (S64 x = 0; x < std::ssize(grid[y]); ++x)
			{
				if (grid[y][x] == '#')
				{
					add_potential_walls_to_remove(grid, walls, { x, y });
				}
			}
		}
		return walls;
	}

	std::vector<Vector2> check_path(Grid& grid, const Vector2& start, const Vector2& end, const std::pair<Vector2, Vector2>& wall)
	{
		const char old_first = grid[wall.first.y][wall.first.x];
		const char old_second = grid[wall.second.y][wall.second.x];
		grid[wall.first.y][wall.first.x] = '1';
		grid[wall.second.y][wall.second.x] = '2';
		const std::vector<Vector2> path = get_shortest_path(grid, start, end);
		grid[wall.first.y][wall.first.x] = old_first;
		grid[wall.second.y][wall.second.x] = old_second;
		return path;
	}
}

SOLVE
{
	Grid grid = read_input(std::cin);

	const Vector2 start = find_position(grid, 'S');
	const Vector2 end = find_position(grid, 'E');
	const std::vector<Vector2> path = get_shortest_path(grid, start, end);
	std::cout << path.size() - 1 << '\n';
	for (const Vector2& position : path)
	{
		grid[position.y][position.x] = 'O';
	}
	for (const std::string& line : grid)
	{
		std::cout << line << '\n';
	}
	std::cout << '\n';

	const auto potential_walls = find_potential_walls(grid);
	std::pair<Vector2, Vector2> expected{ {8, 1}, {9, 1} };
	if (potential_walls.contains(expected))
	{
		std::cout << "Found expected wall\n";
	}
	else
	{
		std::cout << "Did not find expected wall\n";
	}

	std::unordered_map<S64, S64> saved_times;
	S64 part_one = 0;
	for (const auto& cheat : potential_walls)
	{
		const auto cheat_path = check_path(grid, start, end, cheat);
		if (cheat_path.empty()) continue;
		if (cheat_path.size() >= path.size()) continue;
		const S64 diff = std::ssize(path) - std::ssize(cheat_path);
		saved_times[diff] += 1;
		if (diff >= 100) ++part_one;
	}
	std::cout << part_one << '\n';
}