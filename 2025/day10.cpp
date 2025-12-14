#include "aoc.h"
#include "aoc/string_helpers.h"

#include <bitset>
#include <unordered_map>

namespace
{
struct Machine
{
    U64 diagram;
    std::vector<U64> schematics;
};

U64 read_diagram(std::string_view input)
{
    assert(input.size() > 2u);
    U64 diagram = 0ull;
    for (S64 i = 1; i < std::ssize(input) - 1; ++i)
    {
        if (input[i] == '#')
        {
            diagram |= (1ull << (i - 1));
        }
    }
    return diagram;
}

U64 read_schematic(std::string_view input)
{
    assert(input.size() > 2u);
    input = input.substr(1u, input.size() - 2u);
    const std::vector<std::string_view> parts = aoc::split(input, ',');
    U64 schematic = 0ull;
    for (const std::string_view part : parts)
    {
        const U64 index = aoc::convert_unguarded<U64>(part);
        schematic |= (1ull << index);
    }
    return schematic;
}

std::vector<U64> read_schematics(const std::span<const std::string_view> values)
{
    std::vector<U64> schematics;
    for (const std::string_view value : values)
    {
        schematics.push_back(read_schematic(value));
    }
    return schematics;
}

Machine read_line(const std::string_view line)
{
    std::vector<std::string_view> parts = aoc::split(line, ' ');
    assert(parts.size() >= 3u);
    return {read_diagram(parts.front()),
            read_schematics(std::span<const std::string_view>(parts.begin() + 1, parts.end() - 1))};
}

std::vector<Machine> read_input(std::istream &is)
{
    std::vector<Machine> machines;
    for (std::string line; std::getline(is, line);)
    {
        machines.push_back(read_line(line));
    }
    return machines;
}

U64 apply(const U64 lights, const U64 button)
{
    return lights ^ button;
}

using Cache = std::unordered_map<U64, S64>; // lights -> steps

void find_minimum(const S64 steps, const U64 lights, Cache &cache, const std::span<const U64> schematics)
{
    S64 &cached = cache[lights];
    if (cached != 0 && cached <= steps)
    {
        return;
    }
    cached = steps;
    for (const U64 schematic : schematics)
    {
        const U64 new_lights = apply(lights, schematic);
        find_minimum(steps + 1, new_lights, cache, schematics);
    }
}

S64 find_minimum(const U64 lights, const std::span<const U64> schematics)
{
    Cache cache(schematics.size());
    find_minimum(1, lights, cache, schematics);
    const S64 result = cache[0ull];
    assert(result != 0);
    return result - 1;
}

S64 part_one(const std::span<const Machine> machines)
{
    S64 total = 0;
    for (const Machine &machine : machines)
    {
        total += find_minimum(machine.diagram, std::span<const U64>(machine.schematics));
    }
    return total;
}

} // namespace

TEST_CASE("Read diagram")
{
    const U64 diagram = read_diagram("[.#..#]");
    REQUIRE(diagram == 18ull);
}

TEST_CASE("Apply button")
{
    const U64 lights = read_diagram("[#.....]");
    const U64 button = read_schematic("(0,3,4)");
    const U64 result = apply(lights, button);
    REQUIRE(result == 24ull);
    const U64 expected = read_diagram("[...##..]");
    REQUIRE(result == expected);
}

TEST_CASE("2025 Day 10 Part One")
{
    const std::array schematics = {8ull, 10ull, 4ull, 12ull, 5ull, 3ull};
    const S64 steps = find_minimum(6ull, schematics);
    REQUIRE(steps == 2);
}

 TEST_CASE("2025 Day 10 Part One Example")
{
    const char *const input = "[.##.] (3) (1,3) (2) (2,3) (0,2) (0,1) {3,5,4,7}\n"
                              "[...#.] (0,2,3,4) (2,3) (0,4) (0,1,2) (1,2,3,4) {7,5,12,7,2}\n"
                              "[.###.#] (0,1,2,3,4) (0,3,4) (0,1,2,4,5) (1,2) {10,11,11,5,10,5}";
    std::istringstream iss(input);
    const std::vector<Machine> machines = read_input(iss);
    REQUIRE(part_one(machines) == 7);
}

SOLVE
{
    const std::vector<Machine> machines = read_input(std::cin);
    std::cout << part_one(machines) << '\n';
}
