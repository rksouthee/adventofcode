#include "aoc.h"

#include "aoc/orbit_structure.h"

namespace
{
using Grid = std::vector<std::string>;
const char open_ground = '.';
const char tree = '|';
const char lumberyard = '#';

Grid read_input(std::istream &is)
{
    Grid grid;
    for (std::string line; std::getline(is, line);)
    {
        grid.push_back(std::move(line));
    }
    return grid;
}

std::span<char> get_neighbours(const std::span<char, 8> buffer, const Grid &grid, const S64 x, const S64 y)
{
    auto iter = buffer.begin();
    for (S64 ny = y - 1; ny <= y + 1; ++ny)
    {
        if (ny < 0 || ny >= std::ssize(grid))
        {
            continue;
        }
        const std::string_view line = grid[ny];
        for (S64 nx = x - 1; nx <= x + 1; ++nx)
        {
            if (nx < 0 || nx >= line.size())
            {
                continue;
            }
            if (nx == x && ny == y)
            {
                continue;
            }
            *iter++ = line[nx];
        }
    }
    return {buffer.begin(), iter};
}

char update_item(const Grid &grid, const S64 x, const S64 y)
{
    std::array<char, 8> neighbours_buffer;
    const std::span<char> neighbours = get_neighbours(neighbours_buffer, grid, x, y);
    const char ch = grid[y][x];
    switch (ch)
    {
    case open_ground: {
        int tree_count = 0;
        for (const char nc : neighbours)
        {
            if (nc == tree)
            {
                ++tree_count;
                if (tree_count == 3)
                {
                    return tree;
                }
            }
        }
    }
    break;

    case tree: {
        int lumberyard_count = 0;
        for (const char nc : neighbours)
        {
            if (nc == lumberyard)
            {
                ++lumberyard_count;
                if (lumberyard_count == 3)
                {
                    return lumberyard;
                }
            }
        }
    }
    break;

    case lumberyard: {
        bool has_lumberyard = false;
        bool has_tree = false;
        for (const char nc : neighbours)
        {
            has_lumberyard |= nc == lumberyard;
            has_tree |= nc == tree;
        }

        if (!has_lumberyard || !has_tree)
        {
            return open_ground;
        }
    }
    break;
    }

    return ch;
}

Grid update(const Grid &grid)
{
    Grid new_grid = grid;
    for (S64 y = 0; y < std::ssize(grid); ++y)
    {
        const std::string_view line = grid[y];
        for (S64 x = 0; x < line.size(); ++x)
        {
            new_grid[y][x] = update_item(grid, x, y);
        }
    }
    return new_grid;
}

S64 calculate_resource_value(const Grid &grid)
{
    S64 trees = 0;
    S64 lumberyards = 0;
    for (const std::string &line : grid)
    {
        for (const char ch : line)
        {
            switch (ch)
            {
            case tree:
                ++trees;
                break;
            case lumberyard:
                ++lumberyards;
                break;
            }
        }
    }
    return trees * lumberyards;
}

S64 part_one(const Grid &grid)
{
    const S64 iterations = 10;
    return calculate_resource_value(aoc::power(grid, update, iterations));
}

S64 part_two(const Grid &grid)
{
    const S64 iterations = 1000000000;
    return calculate_resource_value(aoc::iterate_nonterminating_orbit(grid, update, iterations));
}
} // namespace

SOLVE
{
    const Grid grid = read_input(std::cin);

    std::cout << part_one(grid) << '\n';
    std::cout << part_two(grid) << '\n';
}
