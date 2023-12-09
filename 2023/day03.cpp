#include <catch2/catch_test_macros.hpp>

#include <iostream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>

namespace
{
	std::vector<std::string> read_input(std::istream& is)
	{
		using I = std::istream_iterator<std::string>;
		return { I{is}, I{} };
	}

	struct Position
	{
		std::size_t y;
		std::size_t x0;
		std::size_t x1;

		Position(std::size_t y, std::size_t x0, std::size_t x1) :
			y{y},
			x0{x0},
			x1{x1}
		{
		}
	};

	bool is_symbol(const char ch)
	{
		return !(std::isdigit(ch) || ch == '.');
	}

	bool check_row(const std::string& row, const std::size_t x0, const std::size_t x1)
	{
		for (std::size_t x = x0; x < x1; ++x)
		{
			if (is_symbol(row[x]))
			{
				return true;
			}
		}
		return false;
	}

	bool is_part_number(const std::vector<std::string>& schematic, const Position& p)
	{
		const std::string& row = schematic[p.y];
		const std::size_t x_min = p.x0 == 0 ? 0 : p.x0 - 1;
		const std::size_t x_max = p.x1 == row.size() ? p.x1 : p.x1 + 1;

		if (p.y > 0 && check_row(schematic[p.y - 1], x_min, x_max))
		{
			return true;
		}

		if (p.y + 1 < schematic.size() && check_row(schematic[p.y + 1], x_min, x_max))
		{
			return true;
		}

		if (x_min != p.x0 && is_symbol(row[x_min]))
		{
			return true;
		}

		if (x_max != p.x1 && is_symbol(row[x_max - 1]))
		{
			return true;
		}

		return false;
	}

	int to_integer(const std::vector<std::string>& schematic, const Position& p)
	{
		int result = 0;
		const std::string& row = schematic[p.y];
		for (std::size_t x = p.x0; x < p.x1; ++x)
		{
			result = result * 10 + (row[x] - '0');
		}
		return result;
	}

	int sum_part_numbers(const std::vector<std::string>& schematic, const std::vector<Position>& positions)
	{
		int result = 0;
		for (const Position& p : positions)
		{
			if (is_part_number(schematic, p))
			{
				result += to_integer(schematic, p);
			}
		}
		return result;
	}

	std::vector<Position> get_number_positions(const std::vector<std::string>& schematic)
	{
		std::vector<Position> positions;
		for (std::size_t y = 0; y < schematic.size(); ++y)
		{
			const std::string& row = schematic[y];
			std::size_t x = 0;
			while (x != row.size())
			{
				while (x != row.size() && !std::isdigit(row[x]))
				{
					++x;
				}

				if (x == row.size())
				{
					continue;
				}

				std::size_t start = x;
				do
				{
					++x;
				}
				while (x != row.size() && std::isdigit(row[x]));
				positions.emplace_back(y, start, x);
			}
		}
		return positions;
	}

	int part_one(const std::vector<std::string>& schematic, const std::vector<Position>& positions)
	{
		return sum_part_numbers(schematic, positions);
	}

	void update_row(const std::string& row, const std::size_t x_min, const std::size_t x_max, std::unordered_map<const char*, std::vector<Position>>& gears, const Position& p)
	{
		for (std::size_t x = x_min; x < x_max; ++x)
		{
			if (row[x] == '*')
			{
				gears[&row[x]].push_back(p);
			}
		}
	}

	void find_gears(const std::vector<std::string>& schematic, const Position& p, std::unordered_map<const char*, std::vector<Position>>& gears)
	{
		const std::string& row = schematic[p.y];
		const std::size_t x_min = p.x0 == 0 ? p.x0 : p.x0 - 1;
		const std::size_t x_max = p.x1 == row.size() ? p.x1 : p.x1 + 1;

		if (p.y > 0)
		{
			update_row(schematic[p.y - 1], x_min, x_max, gears, p);
		}

		if (p.y + 1 < schematic.size())
		{
			update_row(schematic[p.y + 1], x_min, x_max, gears, p);
		}

		if (x_min != p.x0 && row[x_min] == '*')
		{
			gears[&row[x_min]].push_back(p);
		}

		if (x_max != p.x1 && row[x_max - 1] == '*')
		{
			gears[&row[x_max - 1]].push_back(p);
		}
	}

	std::unordered_map<const char*, std::vector<Position>> find_all_gears(const std::vector<std::string>& schematic, const std::vector<Position>& positions)
	{
		std::unordered_map<const char*, std::vector<Position>> gears;
		for (const Position& p : positions)
		{
			find_gears(schematic, p, gears);
		}
		return gears;
	}

	int sum_gear_ratios(const std::vector<std::string>& schematic, const std::unordered_map<const char*, std::vector<Position>>& gears)
	{
		int sum = 0;
		for (const std::pair<const char* const, std::vector<Position>>& gear : gears)
		{
			if (gear.second.size() != 2)
			{
				continue;
			}

			const int first = to_integer(schematic, gear.second.front());
			const int second = to_integer(schematic, gear.second.back());
			const int product = first * second;
			sum += product;
		}
		return sum;
	}

	int part_two(const std::vector<std::string>& schematic, const std::vector<Position>& positions)
	{
		const std::unordered_map<const char*, std::vector<Position>> gears = find_all_gears(schematic, positions);
		return sum_gear_ratios(schematic, gears);
	}
}

TEST_CASE("is_symbol", "[aoc]")
{
	REQUIRE(is_symbol('*'));
}

extern "C" void solve(std::istream& is)
{
	const std::vector<std::string> schematic = read_input(is);
	const std::vector<Position> positions = get_number_positions(schematic);

	std::cout << part_one(schematic, positions) << std::endl;
	std::cout << part_two(schematic, positions) << std::endl;
}
