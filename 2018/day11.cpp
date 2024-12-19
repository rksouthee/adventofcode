#include "aoc.h"
#include "aoc/vector.h"

#include <catch2/catch_test_macros.hpp>

#include <iostream>

using aoc::Vector2;

namespace
{
	S64 get_power_level(const S64 x, const S64 y, const S64 grid_serial_number)
	{
		const S64 rack_id = x + 10;
		S64 power_level = rack_id * y;
		power_level += grid_serial_number;
		power_level *= rack_id;
		power_level = (power_level / 100) % 10;
		power_level -= 5;
		return power_level;
	}

	using Cube = std::vector<std::vector<std::vector<S64>>>;

	Cube get_power_levels(const S64 grid_serial_number)
	{
		Cube power_levels;
		for (S64 y = 0; y < 300; ++y)
		{
			std::vector<std::vector<S64>>& row = power_levels.emplace_back(300);
			for (S64 x = 0; x < 300; ++x)
			{
				row[x].push_back(get_power_level(x + 1, y + 1, grid_serial_number));
			}
		}

		for (S64 y = 0; y < 300; ++y)
		{
			std::vector<std::vector<S64>>& row = power_levels[y];
			for (S64 x = 0; x < 300; ++x)
			{
				for (S64 size = 2; size <= 300; ++size)
				{
					if (x + size > 300 || y + size > 300) break;
					S64 value = row[x][size - 2];
					const S64 last_column = x + size - 1;
					const S64 last_row = y + size - 1;
					for (S64 i = 0; i < size; ++i)
					{
						value += power_levels[y + i][last_column][0];
					}
					for (S64 i = 0; i < size - 1; ++i)
					{
						value += power_levels[last_row][x + i][0];
					}
					row[x].push_back(value);
				}
			}
		}

		return power_levels;
	}

	Vector2 part_one(const Cube& power_levels)
	{
		Vector2 point{};
		S64 max = 0;
		for (S64 y = 0; y < 298; ++y)
		{
			for (S64 x = 0; x < 298; ++x)
			{
				const S64 value = power_levels[y][x][2];
				if (value > max)
				{
					max = value;
					point = { x + 1, y + 1 };
				}
			}
		}
		return point;
	}

	std::pair<Vector2, S64> part_two(const Cube& power_levels)
	{
		Vector2 point;
		S64 size = 0;
		S64 max = 0;
		for (S64 y = 0; y < 300; ++y)
		{
			for (S64 x = 0; x < 300; ++x)
			{
				const auto& data = power_levels[y][x];
				for (S64 i = 0; i < std::ssize(data); ++i)
				{
					if (data[i] > max)
					{
						max = data[i];
						point = { x + 1, y + 1 };
						size = i + 1;
					}
				}
			}
		}
		return { point, size };
	}
}

TEST_CASE("2018 day 11 part 1", "[2018][day11][part1]")
{
	REQUIRE(get_power_level(3, 5, 8) == 4);
	REQUIRE(get_power_level(122, 79, 57) == -5);
	REQUIRE(get_power_level(217, 196, 39) == 0);
	REQUIRE(get_power_level(101, 153, 71) == 4);
}

SOLVE
{
	S64 grid_serial_number;
	std::cin >> grid_serial_number;
	const auto power_levels = get_power_levels(grid_serial_number);

	const auto point = part_one(power_levels);
	std::cout << point.x << ',' << point.y << '\n';

	const auto pair = part_two(power_levels);
	std::cout << pair.first.x << ',' << pair.first.y << ',' << pair.second << '\n';
}