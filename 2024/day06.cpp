#include "aoc.h"
#include "aoc/orbit_structure.h"
#include "aoc/vector.h"

#include <iostream>
#include <string>
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

	Vector2 find_guard(const Grid& grid)
	{
		for (S64 y = 0; y < std::ssize(grid); ++y)
		{
			for (S64 x = 0; x < std::ssize(grid[y]); ++x)
			{
				if (grid[y][x] == '^')
				{
					return { .x = x, .y = y };
				}
			}
		}
		return { .x = -1, .y = -1 };
	}

	Vector2 turn(const Vector2& p)
	{
		if (p == Vector2::east)
		{
			return Vector2::south;
		}

		if (p == Vector2::south)
		{
			return Vector2::west;
		}

		if (p == Vector2::west)
		{
			return Vector2::north;
		}

		return Vector2::east;
	}

	bool is_valid(const Grid& grid, const Vector2& p)
	{
		return p.y >= 0 && p.y < std::ssize(grid) && p.x >= 0 && p.x < std::ssize(grid[p.y]);
	}

	struct Guard
	{
		Vector2 position;
		Vector2 direction;
	};

	Guard step(const Grid& grid, const Guard& guard)
	{
		const Vector2 next = guard.position + guard.direction;
		if (!is_valid(grid, next))
		{
			return { .position = next, .direction = guard.direction };
		}

		if (grid[next.y][next.x] == '#')
		{
			return { .position = guard.position, .direction = turn(guard.direction) };
		}

		return { .position = next, .direction = guard.direction };
	}

	std::unordered_set<S64> get_visited(const Grid& grid, Guard guard)
	{
		std::unordered_set<S64> visited;
		if (grid.empty())
		{
			return visited;
		}

		const S64 width = std::ssize(grid[0]);
		while (is_valid(grid, guard.position))
		{
			visited.insert(guard.position.y * width + guard.position.x);
			guard = step(grid, guard);
		}

		return visited;
	}

	struct State
	{
		const Grid* grid;
		Guard guard;
	};
	
	bool operator==(const State& lhs, const State& rhs)
	{
		return lhs.guard.position == rhs.guard.position && lhs.guard.direction == rhs.guard.direction;
	}

	struct Mover
	{
		State operator()(State state) const
		{
			state.guard = step(*state.grid, state.guard);
			return state;
		}
	};

	struct Is_valid
	{
		bool operator()(const State& state) const
		{
			return is_valid(*state.grid, state.guard.position);
		}
	};

	U64 part_two(Grid grid, const Guard& guard, const std::unordered_set<S64>& visited)
	{
		U64 result = 0;
		if (grid.empty())
		{
			return result;
		}

		const S64 width = std::ssize(grid[0]);
		for (const S64 p : visited)
		{
			const S64 y = p / width;
			const S64 x = p % width;
			if (grid[y][x] != '.')
			{
				continue;
			}

			grid[y][x] = '#';
			result += !aoc::terminating(State{ .grid = &grid, .guard = guard }, Mover{}, Is_valid{});
			grid[y][x] = '.';
		}

		return result;
	}
}

SOLVE
{
	const Grid grid = read_input(std::cin);
	const Vector2 start = find_guard(grid);
	const Guard guard = { .position = start, .direction = Vector2::north };
	const std::unordered_set<S64> visited = get_visited(grid, guard);

	std::cout << visited.size() << '\n';
	std::cout << part_two(grid, guard, visited) << '\n';
}