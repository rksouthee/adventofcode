#include "aoc.h"
#include "aoc/vector.h"

#include <unordered_map>
#include <unordered_set>

namespace
{
    using Grid = std::vector<std::string>;

    Grid read_input(std::istream& in)
    {
        Grid grid;
        std::string line;
        while (std::getline(in, line))
        {
            grid.push_back(line);
        }
        return grid;
    }

    aoc::Vector2 find_starting_position(const Grid &grid)
    {
        for (S64 y = 0; y < static_cast<S64>(grid.size()); ++y)
        {
            for (S64 x = 0; x < static_cast<S64>(grid[y].size()); ++x)
            {
                if (grid[y][x] == 'S')
                {
                    return aoc::Vector2{x, y};
                }
            }
        }
        throw std::runtime_error("Starting position 'S' not found in grid.");
    }

    S64 part_one(const Grid &grid)
    {
        const aoc::Vector2 start = find_starting_position(grid);
        std::unordered_set<S64> columns{start.x};
        S64 splits = 0;
        S64 timelines = 0;
        for (S64 y = start.y + 1; y < std::ssize(grid); ++y)
        {
            std::unordered_set<S64> new_columns;
            const std::string &row = grid[y];
            for (const S64 x : columns)
            {
                if (x >= 0 && x < std::ssize(row))
                {
                    if (row[x] == '^')
                    {
                        ++splits;
                        new_columns.insert(x + 1);
                        new_columns.insert(x - 1);
                    }
                    else
                    {
                        new_columns.insert(x);
                    }
                }
            }
            columns = std::move(new_columns);
        }
        return splits;
    }

    S64 part_two(const Grid &grid)
    {
        const aoc::Vector2 start = find_starting_position(grid);
        std::unordered_map<S64, S64> columns{{start.x, 1}};
        for (S64 y = start.y + 1; y < std::ssize(grid); ++y)
        {
            std::unordered_map<S64, S64> new_columns;
            const std::string &row = grid[y];
            for (const auto [x, timeline] : columns)
            {
                if (x >= 0 && x < std::ssize(row))
                {
                    if (row[x] == '^')
                    {
                        new_columns[x + 1] += timeline;
                        new_columns[x - 1] += timeline;
                    }
                    else
                    {
                        new_columns[x] += timeline;
                    }
                }
            }
            columns = std::move(new_columns);
        }

        S64 timelines = 0;
        for (const auto [_, timeline] : columns)
        {
            timelines += timeline;
        }
        return timelines;
    }
} // namespace

TEST_CASE("2025 Day 7 Part One", "[2025][Day07][PartOne]")
{
}

SOLVE
{
    const Grid grid = read_input(std::cin);
    std::cout << part_one(grid) << '\n';
    std::cout << part_two(grid) << '\n';
}
