#include "aoc.h"
#include "aoc/algorithm.h"
#include <numeric>

namespace
{
struct Input
{
    std::vector<std::vector<std::string>> grid;
    std::vector<char> operations;
};

std::vector<std::string> read_input(std::istream &is)
{
    std::vector<std::string> result;
    for (std::string line; std::getline(is, line);)
    {
        result.push_back(line);
    }

    return result;
}

struct Input convert_input(const std::span<const std::string> lines)
{
    Input input;
    std::vector<std::vector<std::string>> &grid = input.grid;
    for (S64 i = 0; i < std::ssize(lines) - 1; ++i)
    {
        std::istringstream iss(lines[i]);
        std::vector<std::string> row(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>{});
        grid.push_back(std::move(row));
    }
    {
        std::istringstream iss(lines.back());
        std::vector<char> operations(std::istream_iterator<char>(iss), std::istream_iterator<char>{});
        input.operations = std::move(operations);
    }
    return input;
}

template <typename Op> S64 reduce(const Input &input, S64 column, Op op, S64 initial)
{
    for (S64 row = 0; row < std::ssize(input.grid); ++row)
    {
        initial = op(initial, std::stoll(input.grid[row][column]));
    }
    return initial;
}

S64 part_one(const std::span<const std::string> lines)
{
    const Input input = convert_input(lines);
    S64 result = 0;
    for (S64 column = 0; column < std::ssize(input.operations); ++column)
    {
        char operation = input.operations[column];
        switch (operation)
        {
        case '+':
            result += reduce(input, column, std::plus<S64>(), 0);
            break;
        case '*':
            result += reduce(input, column, std::multiplies<S64>(), 1);
            break;
        }
    }
    return result;
}

S64 part_two(const std::span<const std::string> lines)
{
    if (lines.empty())
    {
        return 0;
    }

    S64 result = 0;
    S64 max_columns = std::ssize(lines.front());
    for (const std::string &line : lines)
    {
        assert(std::ssize(line) == max_columns);
    }

    std::vector<S64> operands;
    for (S64 i = 0; i < max_columns; ++i)
    {
        S64 column = max_columns - 1 - i;
        S64 value = 0;
        for (S64 i = 0; i < std::ssize(lines) - 1; ++i)
        {
            const char ch = lines[i][column];
            if (std::isdigit(ch))
            {
                value = value * 10 + (ch - '0');
            }
        }

        if (value != 0)
        {
            operands.push_back(value);
        }

        if (lines.back()[column] == '+')
        {
            result += std::accumulate(operands.begin(), operands.end(), 0ll, std::plus<S64>{});
            operands.clear();
        }
        else if (lines.back()[column] == '*')
        {
            result += std::accumulate(operands.begin(), operands.end(), 1ll, std::multiplies<S64>{});
            operands.clear();
        }
    }
    return result;
}
} // namespace

TEST_CASE("2025 Day 6 Part One", "[2025][Day06][PartOne]")
{
}

SOLVE
{
    const std::vector<std::string> input = read_input(std::cin);
    std::cout << part_one(input) << '\n';
    std::cout << part_two(input) << '\n';
}
