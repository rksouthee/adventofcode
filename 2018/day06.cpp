#include "aoc.h"

#include <catch2/catch_test_macros.hpp>

#include <algorithm>
#include <iostream>
#include <limits>
#include <regex>
#include <vector>

#include <cmath>

namespace
{
	struct Point
	{
		int x;
		int y;
	};

	struct Rect
	{
		int right;
		int bottom;
		int left;
		int top;
	};

	bool operator==(const Rect& a, const Rect& b)
	{
		return a.right == b.right && a.bottom == b.bottom && a.left == b.left && a.top == b.top;
	}

	int manhattan_distance(int x0, int y0, int x1, int y1)
	{
		return std::abs(x0 - x1) + std::abs(y0 - y1);
	}

	Rect get_bounds(const std::vector<Point>& points)
	{
		Rect rect = {
			std::numeric_limits<int>::min(),
			std::numeric_limits<int>::min(),
			std::numeric_limits<int>::max(),
			std::numeric_limits<int>::max()
		};
		for (const Point& point : points)
		{
			rect.right = std::max(rect.right, point.x);
			rect.bottom = std::max(rect.bottom, point.y);
			rect.left = std::min(rect.left, point.x);
			rect.top = std::min(rect.top, point.y);
		}
		return rect;
	}

	std::size_t get_closest_point(const int x, const int y, const std::vector<Point>& points)
	{
		using Pair = std::pair<std::size_t, int>;
		std::vector<Pair> distances;
		for (std::size_t i = 0; i < points.size(); ++i)
		{
			const Point& point = points[i];
			distances.emplace_back(i, manhattan_distance(x, y, point.x, point.y));
		}
		std::sort(distances.begin(), distances.end(), [](const Pair& x, const Pair& y) { return x.second < y.second; });
		if (distances[0].second != distances[1].second) return distances[0].first;
		return points.size();
	}

	std::vector<std::size_t> make_grid(const std::vector<Point>& points, const Rect& bounds)
	{
		const int width = bounds.right - bounds.left + 1;
		const int height = bounds.bottom - bounds.top + 1;
		std::vector<std::size_t> index_map(width * height, points.size());
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				index_map[y * width + x] = get_closest_point(x + bounds.left, y + bounds.top, points);
			}
		}
		return index_map;
	}

	bool is_finite(const Point& point, const Rect& bounds)
	{
		return point.x != bounds.right && point.y != bounds.bottom && point.x != bounds.left && point.y != bounds.top;
	}

	std::ptrdiff_t get_area(const std::vector<std::size_t>& map, std::size_t index)
	{
		return std::count(map.begin(), map.end(), index);
	}

	std::ptrdiff_t get_largest_area(const std::vector<std::size_t>& map, const std::vector<Point>& points, const Rect& bounds)
	{
		std::ptrdiff_t largest_area = 0;
		for (std::size_t i = 0; i < points.size(); ++i)
		{
			const Point& point = points[i];
			if (is_finite(point, bounds))
			{
				largest_area = std::max(largest_area, get_area(map, i));
			}
		}
		return largest_area;
	}

	std::vector<Point> read_input(std::istream& is)
	{
		std::vector<Point> points;
		const std::regex re("(\\d+), (\\d+)");
		for (std::string line; std::getline(is, line);)
		{
			if (std::smatch match; std::regex_match(line, match, re))
			{
				points.emplace_back(std::stoi(match[1].str()), std::stoi(match[2].str()));
			}
		}
		return points;
	}

	std::ptrdiff_t part_one(const std::vector<Point>& points)
	{
		const auto bounds = get_bounds(points);
		const auto grid = make_grid(points, bounds);
		return get_largest_area(grid, points, bounds);
	}

	bool is_reachable(const std::vector<Point>& points, int x, int y, int d)
	{
		for (std::size_t i = 1; i < points.size(); ++i)
		{
			d -= manhattan_distance(x, y, points[i].x, points[i].y);
			if (d <= 0) return false;
		}
		return true;
	}

	std::ptrdiff_t part_two(const std::vector<Point>& points, int safe_distance)
	{
		std::ptrdiff_t safe_area = 0;
		if (points.empty()) return safe_area;
		const Point& p = points.front();
		for (int y = p.y - safe_distance; y <= p.y + safe_distance; ++y)
		{
			const int r = safe_distance - std::abs(p.y - y);
			for (int x = p.x - r; x <= p.x + r; ++x)
			{
				int d = safe_distance - manhattan_distance(x, y, p.x, p.y);
				if (is_reachable(points, x, y, d))
				{
					++safe_area;
				}
			}
		}
		return safe_area;
	}
}

TEST_CASE("get bounds", "")
{
	REQUIRE(get_bounds({{1, 1}, {1, 6}, {8, 3}, {3, 4}, {5, 5}, {8, 9}}) == Rect{8, 9, 1, 1});
}

SOLVE
{
	const std::vector<Point> points = read_input(std::cin);
	std::cout << part_one(points) << std::endl;
	/** @todo allow passing this an option to the command line and default if none specified */
	std::cout << part_two(points, points.size() > 32 ? 10000 : 32) << std::endl;
}
