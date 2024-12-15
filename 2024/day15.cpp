#include <algorithm>

#include "aoc.h"
#include "aoc/vector.h"

#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <cassert>

using aoc::Vector2;

namespace
{
	using Grid = std::vector<std::string>;
	using Collision_handler = Vector2(*)(Grid&, const Vector2&, const Vector2&);
	constexpr char wall = '#';
	constexpr char empty = '.';
	constexpr char robot = '@';
	constexpr char box = 'O';

	Grid read_grid(std::istream& input)
	{
		Grid grid;
		std::string line;
		while (std::getline(input, line) && !line.empty())
		{
			grid.push_back(line);
		}
		return grid;
	}

	std::string read_moves(std::istream& input)
	{
		std::string moves;
		std::string line;
		while (std::getline(input, line))
		{
			moves += line;
		}
		return moves;
	}

	Vector2 find_robot_position(const Grid& grid)
	{
		for (S64 y = 0; y < std::ssize(grid); ++y)
		{
			for (S64 x = 0; x < std::ssize(grid[y]); ++x)
			{
				if (grid[y][x] == robot)
				{
					return { x, y };
				}
			}
		}
		return { -1, -1 };
	}

	Vector2 handle_collision_with_box(Grid& grid, const Vector2& robot_position, const Vector2& direction)
	{
		Vector2 new_position = robot_position + direction;
		do
		{
			new_position += direction;
		} while (grid[new_position.y][new_position.x] == box);
		if (grid[new_position.y][new_position.x] == wall)
		{
			return robot_position;
		}

		grid[new_position.y][new_position.x] = box;
		grid[robot_position.y][robot_position.x] = empty;
		new_position = robot_position + direction;
		grid[new_position.y][new_position.x] = robot;
		return new_position;
	}

	bool is_box(const Grid& grid, const Vector2& position)
	{
		const char ch = grid[position.y][position.x];
		return ch == box || ch == '[' || ch == ']';
	}

	Vector2 move_robot(Grid& grid, const Vector2& position, const Vector2& direction, const Collision_handler handle_box_collision)
	{
		Vector2 new_position = position + direction;
		if (grid[new_position.y][new_position.x] == wall)
		{
			return position;
		}

		if (grid[new_position.y][new_position.x] == empty)
		{
			grid[new_position.y][new_position.x] = robot;
			grid[position.y][position.x] = empty;
		}
		else
		{
			assert(is_box(grid, new_position));
			new_position = handle_box_collision(grid, position, direction);
		}

		return new_position;
	}

	Vector2 char_to_direction(const char ch)
	{
		switch (ch)
		{
		case '^': return Vector2::north;
		case 'v': return Vector2::south;
		case '<': return Vector2::west;
		case '>': return Vector2::east;
		default: return {};
		}
	}

	S64 move_robot(Grid grid, const std::string& moves, const Collision_handler collision_handler)
	{
		Vector2 robot_position = find_robot_position(grid);
		for (const char move : moves)
		{
			robot_position = move_robot(grid, robot_position, char_to_direction(move), collision_handler);
		}

		S64 result = 0;
		for (S64 y = 0; y < std::ssize(grid); ++y)
		{
			for (S64 x = 0; x < std::ssize(grid[y]); ++x)
			{
				if (grid[y][x] == box || grid[y][x] == '[')
				{
					result += 100 * y + x;
				}
			}
		}
		return result;
	}

	S64 part_one(const Grid& grid, const std::string& moves)
	{
		return move_robot(grid, moves, handle_collision_with_box);
	}

	std::string map_tile(const char tile)
	{
		switch (tile)
		{
		case wall: return std::string(2, wall);
		case box: return "[]";
		case empty: return std::string(2, empty);
		case robot: return "@.";
		default: return "";
		}
	}

	Grid widen_grid(const Grid& grid)
	{
		Grid new_grid;
		for (S64 y = 0; y < std::ssize(grid); ++y)
		{
			std::string& row = new_grid.emplace_back();
			row.reserve(std::ssize(grid[y]) * 2);
			for (S64 x = 0; x < std::ssize(grid[y]); ++x)
			{
				row += map_tile(grid[y][x]);
			}
		}
		return new_grid;
	}

	std::vector<Vector2> get_moves(const Grid& grid, const Vector2& position, const Vector2& direction)
	{
		std::set<Vector2> visited;
		std::set<Vector2> positions_to_move = { position };
		std::vector<Vector2> result = { position };
		while (!positions_to_move.empty())
		{
			std::set<Vector2> new_positions_to_move;
			for (const Vector2 p : positions_to_move)
			{
				const Vector2 target = p + direction;
				if (grid[target.y][target.x] == wall)
				{
					return {};
				}

				if (grid[target.y][target.x] == empty)
				{
					continue;
				}

				assert(is_box(grid, target));
				if (!visited.contains(target)) result.push_back(target);
				visited.insert(target);
				if (grid[target.y][target.x] == '[')
				{
					assert(direction != Vector2::west);
					const Vector2 other_side = target + Vector2::east;
					assert(grid[other_side.y][other_side.x] == ']');
					if (!visited.contains(other_side)) result.push_back(other_side);
					visited.insert(other_side);
					new_positions_to_move.insert(other_side);
					if (direction != Vector2::east)
					{
						new_positions_to_move.insert(target);
					}
				}
				else
				{
					assert(grid[target.y][target.x] == ']');
					assert(direction != Vector2::east);
					const Vector2 other_side = target + Vector2::west;
					assert(grid[other_side.y][other_side.x] == '[');
					if (!visited.contains(other_side)) result.push_back(other_side);
					visited.insert(other_side);
					new_positions_to_move.insert(other_side);
					if (direction != Vector2::west)
					{
						new_positions_to_move.insert(target);
					}
				}
			}
			positions_to_move = std::move(new_positions_to_move);
		}
		return result;
	}

	Vector2 handle_collision_with_wide_box(Grid& grid, const Vector2& position, const Vector2& direction)
	{
		const std::vector<Vector2> positions = get_moves(grid, position, direction);
		for (S64 i = std::ssize(positions); i-- > 0;)
		{
			const Vector2 target = positions[i] + direction;
			std::swap(grid[positions[i].y][positions[i].x], grid[target.y][target.x]);
		}
		if (positions.empty()) return position;
		return position + direction;
	}

	S64 part_two(const Grid& grid, const std::string& moves)
	{
		return move_robot(widen_grid(grid), moves, handle_collision_with_wide_box);
	}
}

SOLVE
{
	const Grid grid = read_grid(std::cin);
	const std::string moves = read_moves(std::cin);

	std::cout << part_one(grid, moves) << '\n';
	std::cout << part_two(grid, moves) << '\n';
}