#include "aoc.h"
#include <aoc/orbit_structure.h>

#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include <cstddef>

namespace
{
	using Grid = std::vector<std::string>;

	Grid read_input(std::istream& is)
	{
		using I = std::istream_iterator<std::string>;
		return { I{is}, I{} };
	}

	void drop_boulder_north(Grid& grid, std::size_t x, std::size_t y)
	{
		while (y > 0 && grid[y - 1][x] == '.')
		{
			std::swap(grid[y - 1][x], grid[y][x]);
			--y;
		}
	}

	Grid slide_north(Grid grid)
	{
		for (std::size_t x = 0; x < grid[0].size(); ++x)
		{
			for (std::size_t y = 0; y < grid.size(); ++y)
			{
				if (grid[y][x] == 'O') drop_boulder_north(grid, x, y);
			}
		}
		return grid;
	}

	void drop_boulder_west(Grid& grid, std::size_t x, std::size_t y)
	{
		while (x > 0 && grid[y][x - 1] == '.')
		{
			std::swap(grid[y][x - 1], grid[y][x]);
			--x;
		}
	}

	Grid slide_west(Grid grid)
	{
		for (std::size_t y = 0; y < grid.size(); ++y)
		{
			for (std::size_t x = 0; x < grid.size(); ++x)
			{
				if (grid[y][x] == 'O') drop_boulder_west(grid, x, y);
			}
		}
		return grid;
	}

	void drop_boulder_south(Grid& grid, std::size_t x, std::size_t y)
	{
		while (y + 1 < grid.size() && grid[y + 1][x] == '.')
		{
			std::swap(grid[y + 1][x], grid[y][x]);
			++y;
		}
	}

	Grid slide_south(Grid grid)
	{
		for (std::size_t x = 0; x < grid.front().size(); ++x)
		{
			for (std::size_t y = grid.size(); y > 0; --y)
			{
				if (grid[y - 1][x] == 'O') drop_boulder_south(grid, x, y - 1);
			}
		}
		return grid;
	}

	void drop_boulder_east(Grid& grid, std::size_t x, std::size_t y)
	{
		while (x + 1 < grid[y].size() && grid[y][x + 1] == '.')
		{
			std::swap(grid[y][x], grid[y][x + 1]);
			++x;
		}
	}

	Grid slide_east(Grid grid)
	{
		for (std::size_t y = 0; y < grid.size(); ++y)
		{
			for (std::size_t x = grid[y].size(); x > 0; --x)
			{
				if (grid[y][x - 1] == 'O') drop_boulder_east(grid, x - 1, y);
			}
		}
		return grid;
	}

	std::size_t get_total_load(const std::vector<std::string>& grid)
	{
		std::size_t total_load = 0;
		for (std::size_t y = 0; y < grid.size(); ++y)
		{
			std::size_t load = grid.size() - y;
			for (std::size_t x = 0; x < grid[y].size(); ++x)
			{
				if (grid[y][x] == 'O') total_load += load;
			}
		}
		return total_load;
	}

	std::size_t part_one(const std::vector<std::string>& grid)
	{
		return get_total_load(slide_north(grid));
	}

	std::vector<std::string> perform_cycle(std::vector<std::string> grid)
	{
		grid = slide_north(grid);
		grid = slide_west(grid);
		grid = slide_south(grid);
		grid = slide_east(grid);
		return grid;
	}

	std::size_t part_two(const std::vector<std::string>& grid)
	{
		std::size_t iterations = 1000000000;
		auto os = aoc::orbit_structure_nonterminating_orbit(grid, perform_cycle);
		iterations = (iterations - os.handle_size) % (os.cycle_size + 1);
		auto state = aoc::power(os.connection_point, perform_cycle, iterations);
		return get_total_load(state);
	}
}

SOLVE
{
	const std::vector<std::string> grid = read_input(std::cin);

	std::cout << part_one(grid) << std::endl;
	std::cout << part_two(grid) << std::endl;
}
