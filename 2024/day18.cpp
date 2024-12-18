#include "aoc.h"
#include "aoc/vector.h"

#include <iostream>
#include <queue>
#include <regex>
#include <span>
#include <string>
#include <vector>

using aoc::Vector2;

namespace
{
	using Grid = std::vector<U8>;

	Vector2 parse_point(const std::string& line)
	{
		static const std::regex regex(R"((\d+),(\d+))");
		std::smatch match;
		std::regex_match(line, match, regex);
		return { std::stoll(match[1]), std::stoll(match[2]) };
	}

	std::vector<Vector2> read_input(std::istream& input)
	{
		std::vector<Vector2> points;
		std::string line;
		while (std::getline(input, line))
		{
			points.push_back(parse_point(line));
		}
		return points;
	}

	Grid make_grid(std::span<const Vector2> points, const S64 width, const S64 height)
	{
		Grid grid(width * height, 0);
		for (const Vector2& point : points)
		{
			const S64 index = point.y * width + point.x;
			grid[index] = 1;
		}
		return grid;
	}

	S64 to_index(const S64 width, const Vector2& point)
	{
		return point.y * width + point.x;
	}

	bool is_valid(const Grid& grid, const S64 width, const Vector2& point)
	{
		return point.x >= 0 && point.x < width && point.y >= 0 && point.y < std::ssize(grid) / width;
	}

	S64 shortest_path(const Grid& grid, const S64 width, const Vector2& start, const Vector2& end)
	{
		std::vector<bool> visited(std::size(grid), false);
		std::vector<S64> distances(std::ssize(grid), 0);
		S64 index = to_index(width, start);
		distances[index] = 0;
		visited[index] = true;
		std::queue<Vector2> queue;
		queue.push(start);

		const Vector2 directions[] = { Vector2::east, Vector2::south, Vector2::west, Vector2::north };
		while (!queue.empty())
		{
			const Vector2 p = queue.front();
			queue.pop();
			if (p == end)
			{
				break;
			}
			index = to_index(width, p);
			for (const Vector2 direction : directions)
			{
				const Vector2 next = p + direction;
				const S64 next_index = to_index(width, next);
				if (is_valid(grid, width, next) && grid[next_index] != 1 && !visited[next_index])
				{
					visited[next_index] = true;
					distances[next_index] = distances[index] + 1;
					queue.push(next);
				}
			}
		}

		return distances[to_index(width, end)];
	}
#if 1
	constexpr S64 width = 71;
	constexpr S64 height = 71;
	constexpr S64 num_points = 1024;
#else
	constexpr S64 width = 7;
	constexpr S64 height = 7;
	constexpr S64 num_points = 12;
#endif

	S64 part_one(const std::vector<Vector2>& points)
	{
		const Grid grid = make_grid(std::span{ points.data(), num_points }, width, height);
		return shortest_path(grid, width, { 0, 0 }, { width - 1, height - 1 });
	}

	Vector2 part_two(const std::vector<Vector2>& points)
	{
		Grid grid = make_grid(std::span{ points.data(), num_points }, width, height);
		for (U64 i = num_points + 1; i < std::size(points); ++i)
		{
			grid[to_index(width, points[i])] = 1;
			if (shortest_path(grid, width, { 0, 0 }, { width - 1, height - 1 }) == 0)
			{
				return points[i];
			}
		}
		return {};
	}
}

SOLVE
{
	const std::vector<Vector2> points = read_input(std::cin);
	std::cout << part_one(points) << '\n';
	const Vector2 point = part_two(points);
	std::cout << point.x << ',' << point.y << '\n';
}