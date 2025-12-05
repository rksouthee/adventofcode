#include "aoc.h"
#include "aoc/string_helpers.h"

namespace
{

struct Range
{
    S64 lo;
    S64 hi;

    auto operator<=>(const Range &) const noexcept = default;
};

std::ostream &operator<<(std::ostream &os, const Range &range)
{
    return os << range.lo << '-' << range.hi;
}

struct Database
{
    std::vector<Range> ranges;
    std::vector<S64> ingredients;
};

Database read_input(std::istream &is)
{
    std::vector<Range> ranges;
    for (std::string line; std::getline(is, line) && !line.empty();)
    {
        const auto parts = aoc::split(line, '-');
        assert(parts.size() == 2u);
        const S64 lo = aoc::convert_unguarded<S64>(parts[0]);
        const S64 hi = aoc::convert_unguarded<S64>(parts[1]);
        assert(lo <= hi);
        ranges.emplace_back(lo, hi);
    }

    std::vector<S64> ingredients(std::istream_iterator<S64>(is), std::istream_iterator<S64>{});
    return {std::move(ranges), std::move(ingredients)};
}

bool is_fresh(const Range &range, const S64 id)
{
    return id >= range.lo && id <= range.hi;
}

bool is_fresh(const Database database, const S64 id)
{
    for (const Range &range : database.ranges)
    {
        if (is_fresh(range, id))
        {
            return true;
        }
    }
    return false;
}

S64 part_one(const Database &database)
{
    S64 count = 0;
    for (const S64 id : database.ingredients)
    {
        if (is_fresh(database, id))
        {
            ++count;
        }
    }
    return count;
}

bool can_merge(const Range &a, const Range &b)
{
    assert(a.lo <= b.lo);
    if (b.lo >= a.lo && b.lo <= a.hi)
    {
        return true;
    }
    return false;
}

Range merge(const Range &a, const Range &b)
{
    assert(a.lo <= b.lo);
    const S64 lo = a.lo;
    const S64 hi = std::max(a.hi, b.hi);
    return {lo, hi};
}

std::vector<Range> merge_ranges(const std::vector<Range> &ranges)
{
    std::vector<Range> merged;
    for (const Range &range : ranges)
    {
        if (merged.empty())
        {
            merged.push_back(range);
            continue;
        }
        Range &last = merged.back();
        if (can_merge(last, range))
        {
            last = merge(last, range);
        }
        else
        {
            merged.push_back(range);
        }
    }
    return merged;
}

S64 part_two(const Database &database)
{
    std::vector<Range> ranges = database.ranges;
    std::ranges::sort(ranges);
    while (true)
    {
        const std::vector<Range> merged = merge_ranges(ranges);
        if (merged.size() == ranges.size())
        {
            break;
        }
        ranges = merged;
    }

    S64 count = 0;
    for (const Range &range : ranges)
    {
        count += (range.hi - range.lo) + 1;
    }
    return count;
}

} // namespace

TEST_CASE("2025 Day 5 Part One", "[2025][Day05][PartOne]")
{
}

SOLVE
{
    const Database database = read_input(std::cin);
    std::cout << part_one(database) << '\n';
    std::cout << part_two(database) << '\n';
    //       359,913,027,576,323 too high
    // 9,223,372,036,854,775,807
}
