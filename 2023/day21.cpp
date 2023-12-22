#include "aoc.h"

#include <iostream>
#include <set>
#include <string>
#include <vector>

#include <cassert>
#include <cstdint>

namespace
{
	using Grid = std::vector<std::string>;

	struct Point
	{
		std::uint32_t x;
		std::uint32_t y;

		std::strong_ordering operator<=>(const Point&) const = default;
	};

	Grid read_input(std::istream& is)
	{
		using I = std::istream_iterator<std::string>;
		return { I{is}, I{} };
	}

	bool is_open(const Grid& grid, std::uint32_t x, std::uint32_t y)
	{
		return grid[y][x] != '#';
	}

	std::set<Point> get_next_points(const Grid& grid, const std::set<Point>& curr)
	{
		std::set<Point> points;
		for (const Point& point : curr)
		{
			if (point.x + 1 < grid[point.y].size() && is_open(grid, point.x + 1, point.y))
				points.emplace(point.x + 1, point.y);
			if (point.y + 1 < grid.size() && is_open(grid, point.x, point.y + 1))
				points.emplace(point.x, point.y + 1);
			if (point.x > 0 && is_open(grid, point.x - 1, point.y))
				points.emplace(point.x - 1, point.y);
			if (point.y > 0 && is_open(grid, point.x, point.y - 1))
				points.emplace(point.x, point.y - 1);
		}
		return points;
	}

	Point get_starting_point(const Grid& grid)
	{
		for (std::uint32_t y = 0; y < grid.size(); ++y)
		{
			for (std::uint32_t x = 0; x < grid[y].size(); ++x)
			{
				if (grid[y][x] == 'S') return { x, y };
			}
		}
		assert(!"Unable to find starting position!");
		return { 0, 0 };
	}

	std::size_t part_one(const Grid& grid, std::size_t steps)
	{
		std::set<Point> points = { get_starting_point(grid) };
		for (std::size_t step = 0; step < steps; ++step)
		{
			points = get_next_points(grid, points);
		}
		return points.size();
	}
}

SOLVE
{
	const Grid grid = read_input(std::cin);

	std::cout << part_one(grid, 64) << std::endl;
}
