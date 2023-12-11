#include "aoc.h"
#include <catch2/catch_test_macros.hpp>

#include <iostream>
#include <iterator>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include <cstddef>

namespace
{
	struct Point
	{
		int x;
		int y;

		std::strong_ordering operator<=>(const Point&) const = default;
	};

	using Grid = std::vector<std::string>;

	Grid read_input(std::istream& is)
	{
		using I = std::istream_iterator<std::string>;
		return { I{is}, I{} };
	}

	Point get_starting_point(const Grid& grid)
	{
		for (int y = 0; y < (int)grid.size(); ++y)
		{
			for (int x = 0; x < (int)grid[y].size(); ++x)
			{
				if (grid[y][x] == 'S') return {x, y};
			}
		}
		return {};
	}

	std::size_t get_width(const Grid& grid)
	{
		return grid.front().size();
	}

	std::size_t get_height(const Grid& grid)
	{
		return grid.size();
	}

	char get_value(const Grid& grid, const Point p)
	{
		return grid[p.y][p.x];
	}

	/**
	 * Since each pipe is connected to exactly two other pipes we can search the 4
	 * points around the starting position to see what pipes it connects to.
	 */
	std::pair<Point, Point> get_starting_neighbours(const Grid& grid, const Point p)
	{
		Point r[2];
		std::size_t i = 0;
		const auto add_if_connected = [&r, &i, &grid] (Point q, char p0, char p1, char p2)
		{
			const char ch = get_value(grid, q);
			if (ch == p0 || ch == p1 || ch == p2) r[i++] = q;
		};
		if (p.x + 1 < (int)get_width(grid)) add_if_connected({p.x + 1, p.y}, 'J', '7', '-');
		if (p.x > 0) add_if_connected({p.x - 1, p.y}, 'F', 'L', '-');
		if (p.y + 1 < (int)get_height(grid)) add_if_connected({p.x, p.y + 1}, '|', 'L', 'J');
		if (p.y > 0) add_if_connected({p.x, p.y - 1}, '|', 'F', '7');
		return { r[0], r[1] };
	}

	char get_starting_pipe(const Point start, const Point p0, const Point p1)
	{
		if (p0.x > start.x)
		{
			if (p1.x < start.x) return '-';
			if (p1.y < start.y) return 'L';
			if (p1.y > start.y) return 'F';
		}
		if (p0.x < start.x)
		{
			if (p1.x > start.x) return '-';
			if (p1.y < start.y) return 'J';
			if (p1.y > start.y) return '7';
		}
		if (p0.y < start.y)
		{
			if (p1.x > start.x) return 'L';
			if (p1.x < start.x) return 'J';
			if (p1.y > start.y) return '|';
		}
		if (p0.y > start.y)
		{
			if (p1.x > start.x) return 'L';
			if (p1.x < start.x) return 'J';
			if (p1.y < start.y) return '|';
		}
		return '\0';
	}

	std::pair<Point, Point> get_neighbours(const Grid& grid, const Point p)
	{
		switch (get_value(grid, p))
		{
		case '|': return { {p.x, p.y + 1}, {p.x, p.y - 1} };
		case '-': return { {p.x + 1, p.y}, {p.x - 1, p.y} };
		case 'L': return { {p.x, p.y - 1}, {p.x + 1, p.y} };
		case 'J': return { {p.x, p.y - 1}, {p.x - 1, p.y} };
		case '7': return { {p.x, p.y + 1}, {p.x - 1, p.y} };
		case 'F': return { {p.x, p.y + 1}, {p.x + 1, p.y} };
		}
		return { p, p };
	}

	Point get_next_point(const Grid& grid, const Point prev, const Point curr)
	{
		std::pair<Point, Point> neighbours = get_neighbours(grid, curr);
		if (neighbours.first == prev) return neighbours.second;
		return neighbours.first;
	}

	bool is_vertical_crossing(char value, char previous)
	{
		switch (value)
		{
		case '|':
		case 'F':
		case 'L':
			return true;
		case 'J':
			return previous == 'L';
		case '7':
			return previous == 'F';
		}
		return false;
	}

	/*
	 * Use a ray casting algorithm to determine if a point inside the polygon define by `boundary`. We test how
	 * many times intersects our polygon and if the number of intersections is odd then the point is within the
	 * polygon.
	 */
	bool is_point_in_polygon(const Grid& grid, const std::set<Point>& boundary, int x, int y, int width)
	{
		int crosses = 0;
		char value = get_value(grid, Point{x, y});
		do
		{
			++x;
			const char this_value = get_value(grid, Point{x, y});
			if (boundary.contains(Point{x, y}) && is_vertical_crossing(this_value, value))
			{
				++crosses;
				value = this_value;
			}
		}
		while (x < width);
		return crosses % 2 == 1;
	}
}

TEST_CASE("get starting neighbours", "[neighbours]")
{
	{
		const Grid grid = { "S-7", "|.|", "L-J" };
		const auto p = get_starting_neighbours(grid, {0, 0});
		REQUIRE(p.first == Point{1, 0});
		REQUIRE(p.second == Point{0, 1});
	}
	{
		const Grid grid = { ".FJ", "SJL", "|F-" };
		const auto p = get_starting_neighbours(grid, {0, 1});
		REQUIRE(p.first == Point{1, 1});
		REQUIRE(p.second == Point{0, 2});
	}
	{
		const Grid grid = { "J|F", "7SL", "J|F" };
		const auto p = get_starting_neighbours(grid, {1, 1});
		REQUIRE(p.first == Point{1, 2});
		REQUIRE(p.second == Point{1, 0});
	}
}

SOLVE
{
	Grid grid = read_input(std::cin);
	const Point start = get_starting_point(grid);
	std::pair<Point, Point> neighbours = get_starting_neighbours(grid, start);
	grid[start.y][start.x] = get_starting_pipe(start, neighbours.first, neighbours.second);

	/*
	 * Starting from `start`, we go through and each of the pipes in the loop to our set. The maximum path is then
	 * half the distance of the loop. The set of points determines the boundary of the simple polygon which we can
	 * use to determine if a point is 'inside' the polygon.
	 */
	std::set<Point> points = { start };
	Point curr = neighbours.first;
	Point prev = start;
	while (curr != start)
	{
		points.insert(curr);
		Point temp = get_next_point(grid, prev, curr);
		prev = curr;
		curr = temp;
	}

	// --- Part One ---
	std::cout << points.size() / 2 << std::endl;

	// Replace the pipes not on the loop with '.' tiles.
	for (int y = 0; y < (int)grid.size(); ++y)
	{
		for (int x = 0; x < (int)grid[y].size(); ++x)
		{
			if (points.contains(Point{x, y})) continue;
			grid[y][x] = '.';
		}
	}

	/*
	 * We go through and check all the '.' tiles and see if they are inside the polygon defined by the boundary
	 * set `points`.
	 */
	std::size_t enclosed_tiles = 0;
	for (int y = 0; y < (int)grid.size(); ++y)
	{
		for (int x = 0; x < (int)grid[y].size(); ++x)
		{
			if (grid[y][x] != '.') continue;
			if (is_point_in_polygon(grid, points, x, y, grid[y].size())) ++enclosed_tiles;
		}
	}

	// --- Part Two ---
	std::cout << enclosed_tiles << std::endl;
}
