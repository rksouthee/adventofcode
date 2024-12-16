#include "aoc.h"
#include "aoc/vector.h"

#include <iostream>
#include <queue>
#include <string>
#include <vector>

using aoc::Vector2;

namespace
{
	using Grid = std::vector<std::string>;
	constexpr char wall = '#';

	Grid read_grid(std::istream& input)
	{
		Grid grid;
		std::string line;
		while (std::getline(input, line))
		{
			grid.push_back(line);
		}
		return grid;
	}

	Vector2 find_position(const Grid& grid, const char c)
	{
		for (S64 y = 0; y < std::ssize(grid); ++y)
		{
			for (S64 x = 0; x < std::ssize(grid[y]); ++x)
			{
				if (grid[y][x] == c)
				{
					return { x, y };
				}
			}
		}
		return { -1, -1 };
	}

	bool is_empty(const Grid& grid, const Vector2& position)
	{
		if (position.y < 0 || position.y >= std::ssize(grid) || position.x < 0 || position.x >= std::ssize(grid[position.y]))
		{
			return false;
		}

		return grid[position.y][position.x] != wall;
	}

	struct State
	{
		Vector2 position;
		Vector2 direction;
		S64 distance;
	};

	struct Compare_state
	{
		bool operator()(const State& lhs, const State& rhs) const
		{
			return lhs.distance > rhs.distance;
		}
	};

	std::vector<State> get_neighbours(const Grid& grid, const State& state)
	{
		std::vector<State> neighbours;
		Vector2 direction = state.direction;
		Vector2 target = state.position + direction;
		S64 distance = state.distance + 1;
		if (is_empty(grid, target))
		{
			neighbours.emplace_back(target, direction, distance);
		}

		// turn 90 degrees clockwise
		direction = { state.direction.y, -state.direction.x };
		target = state.position + direction;
		distance = state.distance + 1001;
		if (is_empty(grid, target))
		{
			neighbours.emplace_back(target, direction, distance);
		}

		// turn 90 degrees counter-clockwise
		direction = { -state.direction.y, state.direction.x };
		target = state.position + direction;
		distance = state.distance + 1001;
		if (is_empty(grid, target))
		{
			neighbours.emplace_back(target, direction, distance);
		}

		// turn 180 degrees
		direction = -state.direction;
		target = state.position + direction;
		distance = state.distance + 2001;
		if (is_empty(grid, target))
		{
			neighbours.emplace_back(target, direction, distance);
		}
		return neighbours;
	}

	S64 shortest_path(const Grid& grid, const Vector2& start, Vector2 direction, const Vector2& end)
	{
		constexpr S64 inf = std::numeric_limits<S64>::max() / 2;
		std::vector<std::vector<S64>> distances(std::ssize(grid), std::vector<S64>(std::ssize(grid[0]), inf));
		std::priority_queue<State, std::vector<State>, Compare_state> queue;

		distances[start.y][start.x] = 0;
		queue.emplace(start, direction, 0);

		while (!queue.empty())
		{
			const State state = queue.top();
			queue.pop();

			if (state.position == end)
			{
				break;
			}

			for (const State& neighbour : get_neighbours(grid, state))
			{
				if (neighbour.distance < distances[neighbour.position.y][neighbour.position.x])
				{
					distances[neighbour.position.y][neighbour.position.x] = neighbour.distance;
					queue.push(neighbour);
				}
			}
		}
		return distances[end.y][end.x];
	}

	S64 part_one(const Grid& grid, const Vector2& start, const Vector2& end)
	{
		return shortest_path(grid, start, Vector2::east, end);
	}
}

SOLVE
{
	const Grid grid = read_grid(std::cin);
	const Vector2 start = find_position(grid, 'S');
	const Vector2 end = find_position(grid, 'E');

	std::cout << part_one(grid, start, end) << '\n';
}