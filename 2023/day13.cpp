#include "aoc.h"

#include <iostream>
#include <string>
#include <vector>

#include <cstddef>

namespace
{
	using Grid = std::vector<std::string>;

	std::vector<Grid> read_input(std::istream& is)
	{
		std::vector<Grid> grids;
		Grid grid;
		for (std::string line; std::getline(is, line);)
		{
			if (line.empty()) grids.push_back(std::move(grid));
			else grid.push_back(std::move(line));
		}
		if (!grid.empty()) grids.push_back(std::move(grid));
		return grids;
	}

	int get_symmetric_difference_row(const Grid& grid, std::size_t y0, std::size_t y1)
	{
		int difference = 0;
		for (std::size_t x = 0; x < grid.front().size(); ++x)
		{
			difference += grid[y0][x] == grid[y1][x] ? 0 : 1;
		}
		return difference;
	}

	int get_symmetric_difference_rows(const Grid& grid, std::size_t p)
	{
		std::size_t n = std::min(p, grid.size() - p);
		std::size_t y0 = p - n;
		std::size_t y1 = p + n;
		int difference = 0;
		while (n != 0)
		{
			difference += get_symmetric_difference_row(grid, y0, y1 - 1);
			--n;
			++y0;
			--y1;
		}
		return difference;
	}

	std::size_t find_symmetric_row(const Grid& grid, int difference)
	{
		for (std::size_t i = 1; i < grid.size(); ++i)
		{
			if (get_symmetric_difference_rows(grid, i) == difference) return i;
		}
		return 0;
	}

	int get_symmetric_difference_col(const Grid& grid, std::size_t x0, std::size_t x1)
	{
		int difference = 0;
		for (const std::string& row : grid)
		{
			difference += row[x0] == row[x1] ? 0 : 1;
		}
		return difference;
	}

	int get_symmetric_difference_cols(const Grid& grid, std::size_t p)
	{
		std::size_t n = std::min(p, grid.front().size() - p);
		std::size_t x0 = p - n;
		std::size_t x1 = p + n;
		int difference = 0;
		while (n != 0)
		{
			difference += get_symmetric_difference_col(grid, x0, x1 - 1);
			--n;
			++x0;
			--x1;
		}
		return difference;
	}

	std::size_t find_symmetric_col(const Grid& grid, int difference)
	{
		for (std::size_t i = 1; i < grid.front().size(); ++i)
		{
			if (get_symmetric_difference_cols(grid, i) == difference) return i;
		}
		return 0;
	}

	std::size_t get_summary(const std::vector<Grid>& grids, int difference)
	{
		std::size_t sum = 0;
		for (const Grid& grid : grids)
		{
			sum += find_symmetric_row(grid, difference) * 100;
			sum += find_symmetric_col(grid, difference);
		}
		return sum;
	}

	std::size_t part_one(const std::vector<Grid>& grids)
	{
		return get_summary(grids, 0);
	}

	std::size_t part_two(const std::vector<Grid>& grids)
	{
		return get_summary(grids, 1);
	}
}

SOLVE
{
	const std::vector<Grid> grids = read_input(std::cin);

	std::cout << part_one(grids) << std::endl;
	std::cout << part_two(grids) << std::endl;
}
