#include "aoc.h"
#include "aoc/vector.h"

namespace
{
using Grid = std::vector<std::string>;
Grid read_input(std::istream &is)
{
    Grid grid;
    for (std::string line; std::getline(is, line);)
    {
        grid.push_back(line);
    }
    return grid;
}

std::span<aoc::Vector2> get_neighbours(std::span<aoc::Vector2, 8> buffer, const aoc::Vector2 position, const Grid &grid)
{
    auto iter = buffer.begin();
    for (S64 d_y = -1; d_y <= 1; ++d_y)
    {
        for (S64 d_x = -1; d_x <= 1; ++d_x)
        {
            const aoc::Vector2 neighbour{position.x + d_x, position.y + d_y};
            if (neighbour == position)
            {
                continue;
            }

            if (neighbour.y >= 0 && neighbour.y < std::ssize(grid) && neighbour.x >= 0 &&
                neighbour.x < std::ssize(grid[neighbour.y]))
            {
                *iter++ = neighbour;
            }
        }
    }
    return {buffer.begin(), iter};
}

bool is_accessible(const aoc::Vector2 position, const Grid &grid)
{
    std::array<aoc::Vector2, 8> buffer;
    S64 rolls_found = 0;
    for (const aoc::Vector2 neighbour : get_neighbours(buffer, position, grid))
    {
        if (grid[neighbour.y][neighbour.x] == '@')
        {
            ++rolls_found;
            if (rolls_found >= 4)
            {
                return false;
            }
        }
    }

    return true;
}

std::vector<aoc::Vector2> get_accessible_positions(const Grid &grid)
{
    std::vector<aoc::Vector2> positions;
    for (S64 y = 0; y < std::ssize(grid); ++y)
    {
        for (S64 x = 0; x < std::ssize(grid[y]); ++x)
        {
            const aoc::Vector2 position{x, y};
            if (grid[y][x] == '@' && is_accessible(position, grid))
            {
                positions.push_back(position);
            }
        }
    }
    return positions;
}

S64 part_one(const Grid &grid)
{
    const std::vector<aoc::Vector2> positions = get_accessible_positions(grid);
    return std::ssize(positions);
}

S64 part_two(Grid grid)
{
    S64 count = 0;
    while (true)
    {
        const std::vector<aoc::Vector2> positions = get_accessible_positions(grid);
        if (positions.empty())
        {
            break;
        }
        count += std::ssize(positions);
        for (const aoc::Vector2 position : positions)
        {
            grid[position.y][position.x] = '.';
        }
    }
    return count;
}
} // namespace

TEST_CASE("2025 Day 4 Part One", "[2025][Day04][PartOne]")
{
}

SOLVE
{
    const Grid grid = read_input(std::cin);
    std::cout << part_one(grid) << '\n';
    std::cout << part_two(grid) << '\n';
}
