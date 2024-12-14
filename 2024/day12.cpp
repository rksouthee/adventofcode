#include "aoc.h"
#include "aoc/vector.h"

#include <iostream>
#include <numeric>
#include <ranges>
#include <set>
#include <string>
#include <vector>
#include <catch2/catch_test_macros.hpp>

using aoc::Vector2;

namespace
{
	using Grid = std::vector<std::vector<int>>;
	using Region = std::vector<Vector2>;

	std::vector<std::string> read_input(std::istream& is)
	{
		std::vector<std::string> grid;
		std::string line;
		while (std::getline(is, line))
		{
			grid.push_back(line);
		}
		return grid;
	}

	template <typename C>
	bool is_in_bounds(const C& grid, const Vector2& position)
	{
		return position.y >= 0 && position.y < std::ssize(grid) && position.x >= 0 && position.x < std::ssize(grid[position.y]);
	}

	void find_region(const std::vector<std::string>& grid, const Vector2& position, std::vector<bool>& visited, std::vector<Vector2>& points, const int key)
	{
		const S64 width = std::ssize(grid[0]);
		if (!is_in_bounds(grid, position) || visited[position.y * width + position.x] || grid[position.y][position.x] != key)
		{
			return;
		}

		visited[position.y * width + position.x] = true;
		points.push_back(position);
		const Vector2 cardinal_directions[] = { Vector2::east, Vector2::south, Vector2::west, Vector2::north };
		for (const Vector2& direction : cardinal_directions)
		{
			find_region(grid, position + direction, visited, points, key);
		}
	}

	std::vector<Region> find_regions(const std::vector<std::string>& grid)
	{
		std::vector visited(grid.size() * grid[0].size(), false);
		std::vector<Region> regions;
		for (S64 y = 0; y < std::ssize(grid); ++y)
		{
			for (S64 x = 0; x < std::ssize(grid[y]); ++x)
			{
				if (visited[y * std::ssize(grid[y]) + x])
				{
					continue;
				}
				find_region(grid, { x, y }, visited, regions.emplace_back(), grid[y][x]);
			}
		}
		return regions;
	}

	void fill_region(Grid& grid, const Region& region, const int key)
	{
		for (const auto [x, y] : region)
		{
			grid[y][x] = key;
		}
	}

	Grid make_grid_unique(const std::vector<std::string>& old_grid, const std::vector<Region>& regions)
	{
		Grid grid(std::ssize(old_grid), std::vector<int>(std::ssize(old_grid[0]), 0));
		for (S64 i = 0; i < std::ssize(regions); ++i)
		{
			fill_region(grid, regions[i], static_cast<int>(i));
		}
		return grid;
	}

	std::vector<Vector2> get_boundary_points(const Grid& grid, const Region& region)
	{
		std::vector<Vector2> boundary_points;
		const int key = grid[region[0].y][region[0].x];
		for (const Vector2& point : region)
		{
			const Vector2 cardinal_directions[] = { Vector2::east, Vector2::south, Vector2::west, Vector2::north };
			for (const Vector2& direction : cardinal_directions)
			{
				const Vector2 neighbour = point + direction;
				if (!is_in_bounds(grid, neighbour) || grid[neighbour.y][neighbour.x] != key)
				{
					boundary_points.push_back(neighbour);
				}
			}
		}
		return boundary_points;
	}

	S64 part_one(const Grid& grid, const std::vector<Region>& regions)
	{
		S64 result = 0;
		for (const Region& region : regions)
		{
			const S64 area = std::ssize(region);
			const S64 perimeter = std::ssize(get_boundary_points(grid, region));
			result += area * perimeter;
		}
		return result;
	}

	U8 is_filled(const Grid& grid, const int key, const Vector2& position)
	{
		return is_in_bounds(grid, position) && grid[position.y][position.x] == key;
	}

	constexpr S64 s_table[8] = { 1, 0, 0, 1, 2, 1, 1, 0 };

	S64 is_top_left_corner(const Grid& grid, const int key, const Vector2& position)
	{
		U8 index = 0;
		index += is_filled(grid, key, position + Vector2::east) << 0;
		index += is_filled(grid, key, position + Vector2::south) << 1;
		index += is_filled(grid, key, position) << 2;
		return s_table[index];
	}

	S64 is_top_right_corner(const Grid& grid, const int key, const Vector2& position)
	{
		U8 index = 0;
		index += is_filled(grid, key, position + Vector2::south) << 0;
		index += is_filled(grid, key, position + Vector2::west) << 1;
		index += is_filled(grid, key, position) << 2;
		return s_table[index];
	}

	S64 is_bottom_left_corner(const Grid& grid, const int key, const Vector2& position)
	{
		U8 index = 0;
		index += is_filled(grid, key, position + Vector2::east) << 0;
		index += is_filled(grid, key, position + Vector2::north) << 1;
		index += is_filled(grid, key, position) << 2;
		return s_table[index];
	}

	S64 is_bottom_right_corner(const Grid& grid, const int key, const Vector2& position)
	{
		U8 index = 0;
		index += is_filled(grid, key, position + Vector2::west) << 0;
		index += is_filled(grid, key, position + Vector2::north) << 1;
		index += is_filled(grid, key, position) << 2;
		return s_table[index];
	}

	S64 get_unique_sides(const Grid& grid, const Region& region)
	{
		S64 result = 0;
		std::set<Vector2> visited;
		const int key = grid[region[0].y][region[0].x];
		for (const Vector2& point : region)
		{
			if (const Vector2 top_left = point + Vector2::north_west; !visited.contains(point))
			{
				result += is_top_left_corner(grid, key, top_left);
				visited.insert(point);
			}

			if (const Vector2 top_right = point + Vector2::north_east; !visited.contains(point + Vector2::east))
			{
				result += is_top_right_corner(grid, key, top_right);
				visited.insert(point + Vector2::east);
			}

			if (const Vector2 bottom_left = point + Vector2::south_west; !visited.contains(point + Vector2::south))
			{
				result += is_bottom_left_corner(grid, key, bottom_left);
				visited.insert(point + Vector2::south);
			}

			if (const Vector2 bottom_right = point + Vector2::south_east; !visited.contains(bottom_right))
			{
				result += is_bottom_right_corner(grid, key, bottom_right);
				visited.insert(bottom_right);
			}
		}
		return result;
	}

	S64 part_two(const Grid& grid, const std::vector<Region>& regions)
	{
		S64 result = 0;
		for (const Region& region : regions)
		{
			const S64 area = std::ssize(region);
			const S64 perimeter = get_unique_sides(grid, region);
			result += area * perimeter;
		}
		return result;
	}
}

SOLVE
{
	const std::vector<std::string> input = read_input(std::cin);
	const std::vector<Region> regions = find_regions(input);
	const Grid grid = make_grid_unique(input, regions);
	std::cout << part_one(grid, regions) << '\n';
	std::cout << part_two(grid, regions) << '\n';
}