#include "aoc.h"

namespace
{
std::vector<std::string> read_input(std::istream &is)
{
    std::vector<std::string> lines;
    for (std::string line; std::getline(is, line);)
    {
        if (!line.empty())
        {
            lines.push_back(line);
        }
    }
    return lines;
}

S64 largest_voltage_for_bank(const std::string_view bank, S64 digits)
{
    S64 length = std::ssize(bank);
    assert(length >= digits);
    auto first = bank.data();
    auto last = first + (length - digits + 1);
    S64 result = 0;
    while (digits > 0)
    {
        assert(first != last);
        if (length == digits)
        {
            for (S64 i = 0; i < digits; ++i)
            {
                result = result * 10 + (first[i] - '0');
            }
            return result;
        }
        auto max_it = std::max_element(first, last);
        result = result * 10 + (*max_it - '0');
        first = max_it + 1;
        length = std::ssize(bank) - (first - bank.data());
        --digits;
        last = first + (length - digits + 1);
    }
    return result;
}

S64 sum_voltages(const std::vector<std::string> &banks, const S64 digits)
{
    S64 total_voltage = 0;
    for (const std::string &bank : banks)
    {
        total_voltage += largest_voltage_for_bank(bank, digits);
    }
    return total_voltage;
}

S64 part_one(const std::vector<std::string> &banks)
{
    return sum_voltages(banks, 2);
}

S64 part_two(const std::vector<std::string> &banks)
{
    return sum_voltages(banks, 12);
}
} // namespace

TEST_CASE("2025 Day 03 Part One")
{
    const S64 digits = 2;
    REQUIRE(largest_voltage_for_bank("987654321111111", digits) == 98);
    REQUIRE(largest_voltage_for_bank("811111111111119", digits) == 89);
    REQUIRE(largest_voltage_for_bank("234234234234278", digits) == 78);
    REQUIRE(largest_voltage_for_bank("818181911112111", digits) == 92);
}

TEST_CASE("2025 Day 03 Part Two")
{
    const S64 digits = 12;
    REQUIRE(largest_voltage_for_bank("987654321111111", digits) == 987654321111);
    REQUIRE(largest_voltage_for_bank("811111111111119", digits) == 811111111119);
    REQUIRE(largest_voltage_for_bank("234234234234278", digits) == 434234234278);
    REQUIRE(largest_voltage_for_bank("818181911112111", digits) == 888911112111);
}

SOLVE
{
    const std::vector<std::string> banks = read_input(std::cin);
    std::cout << part_one(banks) << '\n';
    std::cout << part_two(banks) << '\n';
}
