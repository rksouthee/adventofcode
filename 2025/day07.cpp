#include "aoc.h"
#include "aoc/vector.h"

#include <unordered_map>
#include <unordered_set>

namespace
{
using Grid = std::vector<std::string>;

Grid read_input(std::istream &in)
{
    Grid grid;
    for (std::string line; std::getline(in, line);)
    {
        grid.push_back(std::move(line));
    }
    return grid;
}

aoc::Vector2 find_starting_position(const Grid &grid)
{
    for (S64 y = 0; y < std::ssize(grid); ++y)
    {
        for (S64 x = 0; x < std::ssize(grid); ++x)
        {
            if (grid[y][x] == 'S')
            {
                return aoc::Vector2{x, y};
            }
        }
    }
    throw std::runtime_error("Starting position 'S' not found in grid.");
}

struct Result
{
    S64 splits;
    std::unordered_map<S64, S64> timelines;
};

Result evaluate(const Grid &grid)
{
    const aoc::Vector2 start = find_starting_position(grid);
    std::unordered_map<S64, S64> columns{{start.x, 1}};
    S64 splits = 0;
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
                    ++splits;
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
    return Result{splits, std::move(columns)};
}
} // namespace

TEST_CASE("2025 Day 7 Part One", "[2025][Day07][PartOne]")
{
}

SOLVE
{
    const Grid grid = read_input(std::cin);
    const Result result = evaluate(grid);
    std::cout << result.splits << '\n';
    S64 timelines = 0;
    for (const auto [_, timeline] : result.timelines)
    {
        timelines += timeline;
    }
    std::cout << timelines << '\n';
}
