#include "aoc.h"
#include "aoc/string_helpers.h"
#include <iostream>
#include <tuple>

namespace
{
struct Rotation
{
    using Action = std::tuple<S64, bool> (*)(const S64, const S64);
    S64 value;
    S64 revolutions;
    Action action;
};

std::tuple<S64, bool> rotate_left(const S64 position, const S64 rotation);
std::tuple<S64, bool> rotate_right(const S64 position, const S64 rotation);

std::vector<Rotation> read_input(std::istream &is)
{
    std::vector<Rotation> rotations;
    for (std::string line; std::getline(is, line);)
    {
        const std::string_view sv(line.begin() + 1, line.end());
        S64 rotation = aoc::convert_unguarded<S64>(sv);
        const Rotation::Action action = (line[0] == 'L') ? rotate_left : rotate_right;
        rotations.emplace_back(rotation % 100, rotation / 100, action);
    }
    return rotations;
}

std::tuple<S64, bool> rotate_left(const S64 position, const S64 rotation)
{
    assert(rotation >= 0 && rotation < 100);
    const S64 new_position = position - rotation;
    if (new_position < 0)
    {
        return {100 + new_position, position != 0};
    }
    return {new_position, false};
}

std::tuple<S64, bool> rotate_right(const S64 position, const S64 rotation)
{
    assert(rotation >= 0 && rotation < 100);
    const S64 new_position = position + rotation;
    if (new_position >= 100)
    {
        return {new_position - 100, true};
    }
    return {new_position, false};
}

S64 part_one(const std::vector<Rotation> &rotations)
{
    S64 result = 0;
    S64 position = 50;
    for (const Rotation &rotation : rotations)
    {
        position = std::get<0>(rotation.action(position, rotation.value));
        if (position == 0)
        {
            ++result;
        }
    }
    return result;
}

S64 part_two(const std::vector<Rotation> &rotations)
{
    S64 result = 0;
    S64 position = 50;
    for (const Rotation &rotation : rotations)
    {
        result += rotation.revolutions;
        const auto [new_position, wrapped] = rotation.action(position, rotation.value);
        position = new_position;
        if (wrapped || new_position == 0)
        {
            ++result;
        }
    }
    return result;
}
} // namespace

TEST_CASE("2025 Day 01 Part One")
{
}

SOLVE
{
    const std::vector<Rotation> rotations = read_input(std::cin);
    std::cout << part_one(rotations) << '\n';
    std::cout << part_two(rotations) << '\n';
}