#include "aoc.h"

#include <iostream>
#include <iterator>
#include <optional>
#include <set>
#include <string>
#include <vector>

namespace
{
	struct Point
	{
		int x;
		int y;
	};

	using Map = std::vector<std::string>;

	Map read_input(std::istream& is)
	{
		using I = std::istream_iterator<std::string>;
		return { I{is}, I{} };
	}

	bool is_column_empty(const Map& map, int column)
	{
		for (std::size_t y = 0; y < map.size(); ++y)
		{
			if (map[y][column] != '.') return false;
		}
		return true;
	}

	bool is_row_empty(const Map& map, int row)
	{
		for (std::size_t x = 0; x < map[row].size(); ++x)
		{
			if (map[row][x] != '.') return false;
		}
		return true;
	}

	/*
	 * Calculate the minimum distance between two points.
	 *
	 * The minimum distance is the Manhattan distance between the two points plus the number of expanded rows and
	 * columns multiplied by the expansion factor.
	 */
	std::uint64_t get_distance(Point p, Point q, const std::set<int>& rows, const std::set<int>& columns, std::uint64_t expansion)
	{
		int min_x = std::min(p.x, q.x);
		int max_x = std::max(p.x, q.x);
		int min_y = std::min(p.y, q.y);
		int max_y = std::max(p.y, q.y);

		std::uint64_t dx = max_x - min_x;
		std::uint64_t dy = max_y - min_y;

		for (int x = min_x; x <= max_x; ++x)
		{
			if (columns.contains(x)) dx += (expansion - 1);
		}

		for (int y = min_y; y <= max_y; ++y)
		{
			if (rows.contains(y)) dy += (expansion - 1);
		}
		return dx + dy;
	}

	std::uint64_t calculate_min_distances(const std::vector<Point>& galaxies, const std::set<int>& empty_rows, const std::set<int>& empty_columns, std::uint64_t expansion_factor)
	{
		std::uint64_t distances = 0;
		for (std::size_t i = 0; i < galaxies.size(); ++i)
		{
			for (std::size_t j = i + 1; j < galaxies.size(); ++j)
			{
				std::uint64_t distance = get_distance(galaxies[i], galaxies[j], empty_rows, empty_columns, expansion_factor);
				/* std::cout << "  - Between galaxy " << i + 1 << " and galaxy " << j + 1 << ": " << distance << std::endl; */
				distances += distance;
			}
		}
		return distances;
	}
}

SOLVE
{
	std::optional<std::size_t> opt_value;
	if (argc > 1)
	{
		std::cout << "Handling argument " << argv[1] << std::endl;
		opt_value = std::stoull(argv[1]);
	}

	const std::vector<std::string> map = read_input(std::cin);

	std::set<int> empty_columns;
	for (int x = 0; x < (int)map.front().size(); ++x)
	{
		if (is_column_empty(map, x)) empty_columns.insert(x);
	}

	std::set<int> empty_rows;
	for (int y = 0; y < (int)map.size(); ++y)
	{
		if (is_row_empty(map, y)) empty_rows.insert(y);
	}

	std::vector<Point> points;
	for (int y = 0; y < (int)map.size(); ++y)
	{
		for (int x = 0; x < (int)map[y].size(); ++x)
		{
			if (map[y][x] == '#')
			{
				points.emplace_back(x, y);
				/* std::cout << "galaxy " << points.size() << ": (" << x << ", " << y << ")" << std::endl; */
			}
		}
	}

	if (opt_value)
	{
		std::cout << calculate_min_distances(points, empty_rows, empty_columns, *opt_value) << std::endl;
	}
	else
	{
		std::cout << calculate_min_distances(points, empty_rows, empty_columns, 2) << std::endl;
		std::cout << calculate_min_distances(points, empty_rows, empty_columns, 1'000'000) << std::endl;
	}
}
