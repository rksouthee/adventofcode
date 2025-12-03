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

S64 largest_potential(const S64 voltage, const S64 digits)
{
    S64 result = voltage;
    for (S64 i = 0; i < digits; ++i)
    {
        result = result * 10 + 9;
    }
    return result;
}

S64 largest_voltage_for_bank(const std::string_view bank, S64 largest, const S64 voltage, const S64 start, const S64 remaining)
{
    if (remaining == 0)
    {
        return voltage;
    }

    if (start >= std::ssize(bank) || std::ssize(bank) - start < remaining)
    {
        return 0;
    }

    if (largest_potential(voltage, remaining) <= largest)
    {
        return 0;
    }

    for (S64 i = start; i < std::ssize(bank); ++i)
    {
        const S64 new_voltage = voltage * 10 + (bank[i] - '0');
        largest = std::max(largest, largest_voltage_for_bank(bank, largest, new_voltage, i + 1, remaining - 1));
    }
    return largest;
}

S64 largest_voltage_for_bank(const std::string_view bank, const S64 digits)
{
    return largest_voltage_for_bank(bank, 0, 0, 0, digits);
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
    REQUIRE(largest_voltage_for_bank("123", 2) == 23);
    REQUIRE(largest_voltage_for_bank("303", 4) == 0);
    REQUIRE(largest_voltage_for_bank("1001", 3) == 101);
    REQUIRE(largest_voltage_for_bank("909", 2) == 99);
}

SOLVE
{
    const std::vector<std::string> banks = read_input(std::cin);
    std::cout << part_one(banks) << '\n';
    std::cout << part_two(banks) << '\n';
}
