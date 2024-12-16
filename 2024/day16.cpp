#include "aoc.h"
#include "aoc/vector.h"

#include <array>
#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <vector>

using aoc::Vector2;

namespace
{
	using Grid = std::vector<std::string>;
	constexpr char wall = '#';

	enum class Direction : U8
	{
		east = 0,
		south = 1,
		west = 2,
		north = 3
	};

	Vector2 direction_to_vector(const Direction direction)
	{
		switch (direction)
		{
		case Direction::east: return Vector2::east;
		case Direction::south: return Vector2::south;
		case Direction::west: return Vector2::west;
		case Direction::north: return Vector2::north;
		}
		return {};
	}

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
		return { .x = -1, .y = -1 };
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
		Direction direction;
		S64 distance;
	};

	struct Compare_state
	{
		bool operator()(const State& lhs, const State& rhs) const
		{
			return lhs.distance > rhs.distance;
		}
	};

	Direction turn_left(const Direction direction)
	{
		switch (direction)
		{
		case Direction::east: return Direction::north;
		case Direction::south: return Direction::east;
		case Direction::west: return Direction::south;
		case Direction::north: return Direction::west;
		}
		return {};
	}

	Direction turn_right(const Direction direction)
	{
		switch (direction)
		{
		case Direction::east: return Direction::south;
		case Direction::south: return Direction::west;
		case Direction::west: return Direction::north;
		case Direction::north: return Direction::east;
		}
		return {};
	}

	Direction turn_around(const Direction direction)
	{
		switch (direction)
		{
		case Direction::east: return Direction::west;
		case Direction::south: return Direction::north;
		case Direction::west: return Direction::east;
		case Direction::north: return Direction::south;
		}
		return {};
	}

	std::vector<State> get_neighbours(const Grid& grid, const State& state)
	{
		std::vector<State> neighbours;
		Direction direction = state.direction;
		Vector2 target = state.position + direction_to_vector(direction);
		S64 distance = state.distance + 1;
		if (is_empty(grid, target))
		{
			neighbours.emplace_back(target, direction, distance);
		}

		// turn 90 degrees clockwise
		direction = turn_right(state.direction);
		target = state.position + direction_to_vector(direction);
		distance = state.distance + 1001;
		if (is_empty(grid, target))
		{
			neighbours.emplace_back(target, direction, distance);
		}

		// turn 90 degrees counter-clockwise
		direction = turn_left(state.direction);
		target = state.position + direction_to_vector(direction);
		distance = state.distance + 1001;
		if (is_empty(grid, target))
		{
			neighbours.emplace_back(target, direction, distance);
		}

		// turn 180 degrees
		direction = turn_around(state.direction);
		target = state.position + direction_to_vector(direction);
		distance = state.distance + 2001;
		if (is_empty(grid, target))
		{
			neighbours.emplace_back(target, direction, distance);
		}
		return neighbours;
	}

	std::pair<S64, Direction> shortest_path(const Grid& grid, const Vector2& start, Direction direction, const Vector2& end, std::vector<std::vector<std::array<std::vector<State>, 4>>>& parents)
	{
		constexpr S64 inf = std::numeric_limits<S64>::max() / 2;
		const S64 height = std::ssize(grid);
		const S64 width = std::ssize(grid[0]);
		constexpr std::array inf_array = { inf, inf, inf, inf };
		std::vector<std::vector<std::array<S64, 4>>> distances;
		for (S64 y = 0; y < height; ++y)
		{
			std::vector<std::array<S64, 4>>& row = distances.emplace_back();
			for (S64 x = 0; x < width; ++x)
			{
				row.push_back(inf_array);
			}
		}
		std::priority_queue<State, std::vector<State>, Compare_state> queue;

		for (S64 i = 0; i < 4; ++i)
		{
			distances[start.y][start.x][i] = 0;
		}
		queue.emplace(start, direction, 0);

		while (!queue.empty())
		{
			const State state = queue.top();
			queue.pop();

			for (const State& neighbour : get_neighbours(grid, state))
			{
				const auto dir_index = static_cast<S64>(neighbour.direction);
				std::vector<State>& this_parent = parents[neighbour.position.y][neighbour.position.x][dir_index];
				S64& existing_distance = distances[neighbour.position.y][neighbour.position.x][dir_index];
				if (neighbour.distance < existing_distance)
				{
					existing_distance = neighbour.distance;
					queue.push(neighbour);
					this_parent.clear();
					this_parent.push_back(state);
				}
				else if (neighbour.distance == existing_distance)
				{
					this_parent.push_back(state);
				}
			}
		}

		const auto& end_distances = distances[end.y][end.x];
		const auto min_el = std::ranges::min_element(end_distances);
		return { *min_el, static_cast<Direction>(min_el - end_distances.begin()) };
	}

	std::set<Vector2> find_unique_points(const std::vector<std::vector<std::array<std::vector<State>, 4>>>& parents, const State& end)
	{
		std::set points = { end.position };
		std::vector<State> states = parents[end.position.y][end.position.x][static_cast<S64>(end.direction)];
		while (!states.empty())
		{
			const State& state = states.back();
			states.pop_back();
			points.insert(state.position);
			const std::vector<State>& new_states = parents[state.position.y][state.position.x][static_cast<S64>(state.direction)];
			states.insert(states.end(), new_states.begin(), new_states.end());
		}
		return points;
	}

	std::pair<S64, S64> solve(const Grid& grid, const Vector2& start, const Vector2& end)
	{
		const S64 height = std::ssize(grid);
		const S64 width = std::ssize(grid[0]);
		std::vector<std::vector<std::array<std::vector<State>, 4>>> parents;
		for (S64 y = 0; y < height; ++y)
		{
			std::vector<std::array<std::vector<State>, 4>>& row = parents.emplace_back();
			for (S64 x = 0; x < width; ++x)
			{
				row.emplace_back();
			}
		}
		const std::pair<S64, Direction> pair = shortest_path(grid, start, Direction::east, end, parents);
		const std::set<Vector2> points = find_unique_points(parents, State{ end, pair.second, pair.first });
		return { pair.first, points.size() };
	}
}

SOLVE
{
	const Grid grid = read_grid(std::cin);
	const Vector2 start = find_position(grid, 'S');
	const Vector2 end = find_position(grid, 'E');

	const auto [first, second] = solve(grid, start, end);
	std::cout << first << '\n';
	std::cout << second << '\n';
}