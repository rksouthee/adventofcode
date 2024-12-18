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

	constexpr S64 width = 71;
	constexpr S64 height = 71;
	constexpr S64 num_points = 1024;
	constexpr Vector2 start = { .x = 0, .y = 0 };
	constexpr Vector2 end = { .x = width - 1, .y = height - 1 };

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

	Grid make_grid(const std::span<const Vector2> points)
	{
		Grid grid(width * height, 0);
		for (const Vector2& point : points)
		{
			const S64 index = point.y * width + point.x;
			grid[index] = 1;
		}
		return grid;
	}

	S64 to_index(const Vector2& point)
	{
		return point.y * width + point.x;
	}

	bool is_valid(const Vector2& point)
	{
		return point.x >= 0 && point.x < width && point.y >= 0 && point.y < height;
	}

	S64 shortest_path(const Grid& grid)
	{
		std::vector<S64> distances(std::ssize(grid), 0);
		S64 index = to_index(start);
		distances[index] = 0;
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

			index = to_index(p);
			for (const Vector2 direction : directions)
			{
				const Vector2 next = p + direction;
				const S64 next_index = to_index(next);
				if (is_valid(next) && grid[next_index] != 1 && !distances[next_index])
				{
					distances[next_index] = distances[index] + 1;
					queue.push(next);
				}
			}
		}

		return distances[to_index(end)];
	}

	S64 part_one(const std::vector<Vector2>& points)
	{
		const Grid grid = make_grid(std::span{ points.data(), num_points });
		return shortest_path(grid);
	}

	Vector2 part_two(const std::vector<Vector2>& points)
	{
		const auto f = std::ranges::partition_point(points, [&](const Vector2& point)
			{
				const Grid grid = make_grid(std::span{ points.data(), &point + 1 });
				return shortest_path(grid) != 0;
			});
		if (f == std::end(points)) return {};
		return *f;
	}
}

SOLVE
{
	const std::vector<Vector2> points = read_input(std::cin);
	std::cout << part_one(points) << '\n';
	const auto [x, y] = part_two(points);
	std::cout << x << ',' << y << '\n';
}