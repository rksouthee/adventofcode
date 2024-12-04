#include "aoc.h"

#include <iostream>
#include <string>
#include <vector>

namespace
{
	struct Vector2
	{
		std::int64_t x;
		std::int64_t y;

		static const Vector2 east;
		static const Vector2 north_east;
		static const Vector2 north;
		static const Vector2 north_west;
		static const Vector2 west;
		static const Vector2 south_west;
		static const Vector2 south;
		static const Vector2 south_east;
	};

	const Vector2 Vector2::east{ .x = 1, .y = 0 };
	const Vector2 Vector2::north_east{ .x = 1, .y = -1 };
	const Vector2 Vector2::north{ .x = 0, .y = -1 };
	const Vector2 Vector2::north_west{ .x = -1, .y = -1 };
	const Vector2 Vector2::west{ .x = -1, .y = 0 };
	const Vector2 Vector2::south_west{ .x = -1, .y = 1 };
	const Vector2 Vector2::south{ .x = 0, .y = 1 };
	const Vector2 Vector2::south_east{ .x = 1, .y = 1 };

	Vector2 operator+(const Vector2& p, const Vector2& q)
	{
		return { .x = p.x + q.x, .y = p.y + q.y };
	}

	Vector2 operator*(const Vector2& p, const std::size_t k)
	{
		return { .x = p.x * static_cast<std::int64_t>(k), .y = p.y * static_cast<std::int64_t>(k) };
	}

	using Grid = std::vector<std::string>;

	Grid read_input(std::istream& input)
	{
		Grid result;
		std::string line;
		while (std::getline(input, line))
		{
			result.push_back(line);
		}
		return result;
	}

	std::ptrdiff_t matches_word(const Grid& grid, const Vector2 start, const Vector2 direction, const std::string_view word)
	{
		if (const auto [end_x, end_y] = start + direction * (word.size() - 1);
			end_y < 0 || end_y >= static_cast<std::int64_t>(grid.size()) || end_x < 0 || end_x >= static_cast<std::int64_t>(grid[0].size()))
		{
			return 0;
		}

		for (std::size_t i = 0; i < word.size(); ++i)
		{
			if (const auto [x, y] = start + (direction * i);
				grid[y][x] != word[i])
			{
				return 0;
			}
		}

		return 1;
	}

	std::ptrdiff_t matches_word_east(const Grid& grid, const Vector2 start, const std::string_view word)
	{
		return matches_word(grid, start, Vector2::east, word);
	}

	std::ptrdiff_t matches_word_south_east(const Grid& grid, const Vector2 start, const std::string_view word)
	{
		return matches_word(grid, start, Vector2::south_east, word);
	}

	std::ptrdiff_t matches_word_south(const Grid& grid, const Vector2 start, const std::string_view word)
	{
		return matches_word(grid, start, Vector2::south, word);
	}

	std::ptrdiff_t matches_word_south_west(const Grid& grid, const Vector2 start, const std::string_view word)
	{
		return matches_word(grid, start, Vector2::south_west, word);
	}

	std::ptrdiff_t matches_word_west(const Grid& grid, const Vector2 start, const std::string_view word)
	{
		return matches_word(grid, start, Vector2::west, word);
	}

	std::ptrdiff_t matches_word_north_west(const Grid& grid, const Vector2 start, const std::string_view word)
	{
		return matches_word(grid, start, Vector2::north_west, word);
	}

	std::ptrdiff_t matches_word_north(const Grid& grid, const Vector2 start, const std::string_view word)
	{
		return matches_word(grid, start, Vector2::north, word);
	}

	std::ptrdiff_t matches_word_north_east(const Grid& grid, const Vector2 start, const std::string_view word)
	{
		return matches_word(grid, start, Vector2::north_east, word);
	}

	std::ptrdiff_t count_words(const Grid& grid, const Vector2 start, const std::string_view word)
	{
		std::ptrdiff_t result = 0;
		result += matches_word_east(grid, start, word);
		result += matches_word_south_east(grid, start, word);
		result += matches_word_south(grid, start, word);
		result += matches_word_south_west(grid, start, word);
		result += matches_word_west(grid, start, word);
		result += matches_word_north_west(grid, start, word);
		result += matches_word_north(grid, start, word);
		result += matches_word_north_east(grid, start, word);
		return result;
	}

	std::ptrdiff_t part_one(const Grid& grid)
	{
		std::ptrdiff_t result = 0;
		for (std::size_t y = 0; y < grid.size(); ++y)
		{
			const std::string& line = grid[y];
			for (std::size_t x = 0; x < line.size(); ++x)
			{
				const std::ptrdiff_t count = count_words(grid, { .x = static_cast<std::int64_t>(x), .y = static_cast<std::int64_t>(y) }, "XMAS");
				result += count;
			}
		}
		return result;
	}

	std::ptrdiff_t count_cross(const Grid& grid, const Vector2 start, const std::string_view word)
	{
		if (matches_word_south_east(grid, start + Vector2::north_west, word))
		{
			if (matches_word_north_east(grid, start + Vector2::south_west, word))
			{
				return 1;
			}

			if (matches_word_south_west(grid, start + Vector2::north_east, word))
			{
				return 1;
			}
		}
		else if (matches_word_south_west(grid, start + Vector2::north_east, word))
		{
			if (matches_word_north_west(grid, start + Vector2::south_east, word))
			{
				return 1;
			}
		}
		else if (matches_word_north_west(grid, start + Vector2::south_east, word))
		{
			if (matches_word_north_east(grid, start + Vector2::south_west, word))
			{
				return 1;
			}
		}
		return 0;
	}

	std::ptrdiff_t part_two(const Grid& grid)
	{
		std::ptrdiff_t result = 0;
		for (std::size_t y = 1; y < grid.size() - 1; ++y)
		{
			const std::string& line = grid[y];
			for (std::size_t x = 1; x < line.size() - 1; ++x)
			{
				if (grid[y][x] == 'A')
				{
					result += count_cross(grid, { .x = static_cast<std::int64_t>(x), .y = static_cast<std::int64_t>(y) }, "MAS");
				}
			}
		}
		return result;
	}
}

SOLVE
{
	const Grid input = read_input(std::cin);

	std::cout << part_one(input) << '\n';
	std::cout << part_two(input) << '\n';
}