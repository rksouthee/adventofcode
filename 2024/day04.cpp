#include "aoc.h"
#include "aoc/vector.h"

#include <iostream>
#include <string>
#include <vector>

using aoc::Vector2;

namespace
{
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

	S64 matches_word(const Grid& grid, const Vector2 start, const Vector2 direction, const std::string_view word)
	{
		if (const auto [end_x, end_y] = start + direction * (std::ssize(word) - 1);
		    end_y < 0 || end_y >= std::ssize(grid) || end_x < 0 || end_x >= std::ssize(grid[end_y]))
		{
			return 0;
		}

		for (S64 i = 0; i < std::ssize(word); ++i)
		{
			if (const auto [x, y] = start + (direction * i);
			    grid[y][x] != word[i])
			{
				return 0;
			}
		}

		return 1;
	}

	S64 matches_word_east(const Grid& grid, const Vector2 start, const std::string_view word)
	{
		return matches_word(grid, start, Vector2::east, word);
	}

	S64 matches_word_south_east(const Grid& grid, const Vector2 start, const std::string_view word)
	{
		return matches_word(grid, start, Vector2::south_east, word);
	}

	S64 matches_word_south(const Grid& grid, const Vector2 start, const std::string_view word)
	{
		return matches_word(grid, start, Vector2::south, word);
	}

	S64 matches_word_south_west(const Grid& grid, const Vector2 start, const std::string_view word)
	{
		return matches_word(grid, start, Vector2::south_west, word);
	}

	S64 matches_word_west(const Grid& grid, const Vector2 start, const std::string_view word)
	{
		return matches_word(grid, start, Vector2::west, word);
	}

	S64 matches_word_north_west(const Grid& grid, const Vector2 start, const std::string_view word)
	{
		return matches_word(grid, start, Vector2::north_west, word);
	}

	S64 matches_word_north(const Grid& grid, const Vector2 start, const std::string_view word)
	{
		return matches_word(grid, start, Vector2::north, word);
	}

	S64 matches_word_north_east(const Grid& grid, const Vector2 start, const std::string_view word)
	{
		return matches_word(grid, start, Vector2::north_east, word);
	}

	S64 count_words(const Grid& grid, const Vector2 start, const std::string_view word)
	{
		S64 result = 0;
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

	S64 part_one(const Grid& grid)
	{
		S64 result = 0;
		for (S64 y = 0; y < std::ssize(grid); ++y)
		{
			const std::string& line = grid[y];
			for (S64 x = 0; x < std::ssize(line); ++x)
			{
				result += count_words(grid, { .x = x, .y = y }, "XMAS");
			}
		}
		return result;
	}

	S64 count_cross(const Grid& grid, const Vector2 start, const std::string_view word)
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

	S64 part_two(const Grid& grid)
	{
		S64 result = 0;
		for (S64 y = 1; y < std::ssize(grid) - 1; ++y)
		{
			const std::string& line = grid[y];
			for (S64 x = 1; x < std::ssize(line) - 1; ++x)
			{
				if (grid[y][x] == 'A')
				{
					result += count_cross(grid, { .x = x, .y = y }, "MAS");
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