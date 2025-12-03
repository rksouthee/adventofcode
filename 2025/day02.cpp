#include "aoc.h"
#include "aoc/string_helpers.h"

namespace
{
struct Input
{
    std::string data;
    std::vector<std::string_view> ranges;
};

Input read_input(std::istream &is)
{
    Input input;
    if (!std::getline(is, input.data))
    {
        throw std::runtime_error("Failed to read data line");
    }

    input.ranges = aoc::split(input.data, ',');
    return input;
}

bool is_valid_id(const std::string_view id, const S64 k)
{
    if (id.empty() || id.size() % k != 0)
    {
        return true;
    }

    const std::string_view first_section = id.substr(0, k);
    const S64 num_sections = id.size() / k;
    for (S64 section = 1; section < num_sections; ++section)
    {
        const std::string_view sub_id = id.substr(section * k, k);
        if (sub_id != first_section)
        {
            return true;
        }
    }

    return false;
}

template <typename P> S64 count_invalid_ids(const std::string_view range, const P p)
{
    const std::vector<std::string_view> ids = aoc::split(range, '-');
    S64 sum = 0;
    const S64 lo = aoc::convert_unguarded<S64>(ids[0]);
    const S64 hi = aoc::convert_unguarded<S64>(ids[1]);
    for (S64 id = lo; id <= hi; ++id)
    {
        const std::string id_str = std::to_string(id);
        if (!p(id_str))
        {
            AOC_LOG("Invalid ID: " << id);
            sum += id;
        }
    }
    return sum;
}

struct RepeatsTwice
{
    bool operator()(const std::string_view id) const noexcept
    {
        const S64 len = std::ssize(id);
        return len % 2 != 0 || is_valid_id(id, len / 2);
    }
};

template <typename P> S64 sum_invalid_ids(const Input &input, const P p)
{
    S64 sum = 0;
    for (const std::string_view range : input.ranges)
    {
        sum += count_invalid_ids(range, p);
    }
    return sum;
}

S64 part_one(const Input &input)
{
    return sum_invalid_ids(input, RepeatsTwice{});
}

struct RepeatsAtLeastTwice
{
    bool operator()(const std::string_view id) const noexcept
    {
        const S64 len = std::ssize(id);
        for (S64 k = 2; k <= len; ++k)
        {
            if (len % k == 0 && !is_valid_id(id, len / k))
            {
                return false;
            }
        }
        return true;
    }
};

S64 part_two(const Input &input)
{
    return sum_invalid_ids(input, RepeatsAtLeastTwice{});
}
} // namespace

TEST_CASE("2025 Day 02 Part One")
{
    REQUIRE_FALSE(is_valid_id("55", 1));
    REQUIRE_FALSE(is_valid_id("11", 1));
    REQUIRE(is_valid_id("13", 1));

    // Example
    REQUIRE(count_invalid_ids("11-22", RepeatsTwice{}) == 11 + 22);

    // Example Part Two
    REQUIRE(count_invalid_ids("565653-565659", RepeatsAtLeastTwice{}) == 565656);
}

SOLVE
{
    const Input input = read_input(std::cin);
    std::cout << part_one(input) << '\n';
    std::cout << part_two(input) << '\n';
}
