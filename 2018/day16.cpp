#include "aoc.h"
#include "aoc/core.h"

#include <catch2/catch_test_macros.hpp>

#include <array>
#include <bitset>
#include <vector>

namespace
{
enum class Opcode : U16
{
    addr,
    addi,
    mulr,
    muli,
    banr,
    bani,
    borr,
    bori,
    setr,
    seti,
    gtir,
    gtri,
    gtrr,
    eqir,
    eqri,
    eqrr,
};

constexpr U16 num_opcodes = 16;
using Row = std::array<U16, 4>;
using Sample = std::array<Row, 4>;

struct Input
{
    std::vector<Sample> samples;
    std::vector<Row> instructions;
};

Input read_input(std::istream &in)
{
    // Read lines of the form:
    // Before: [3, 2, 1, 1]
    // 9 2 1 2
    // After:  [3, 2, 2, 1]
    // then 3 blank lines, then a series of lines of the form:
    // 9 2 1 2
    Input input;
    std::string line;
    while (std::getline(in, line))
    {
        if (line.empty())
        {
            break;
        }
        Sample sample;
        // Before
        {
            std::sscanf(line.c_str(), "Before: [%hu, %hu, %hu, %hu]", &sample[0][0], &sample[0][1], &sample[0][2],
                        &sample[0][3]);
        }
        // Instruction
        std::getline(in, line);
        {
            std::sscanf(line.c_str(), "%hu %hu %hu %hu", &sample[1][0], &sample[1][1], &sample[1][2], &sample[1][3]);
        }
        // After
        std::getline(in, line);
        {
            std::sscanf(line.c_str(), "After:  [%hu, %hu, %hu, %hu]", &sample[2][0], &sample[2][1], &sample[2][2],
                        &sample[2][3]);
        }
        input.samples.push_back(sample);
        // Blank line
        std::getline(in, line);
    }

    // Now read the program data
    while (std::getline(in, line))
    {
        if (line.empty())
        {
            continue;
        }
        Row instruction;
        std::sscanf(line.c_str(), "%hu %hu %hu %hu", &instruction[0], &instruction[1], &instruction[2],
                    &instruction[3]);
        input.instructions.push_back(instruction);
    }

    return input;
}

Row evaluate(Row registers, const Opcode opcode, const U16 a, const U16 b, const U16 c)
{
    switch (opcode)
    {
    case Opcode::addr:
        registers[c] = registers[a] + registers[b];
        break;

    case Opcode::addi:
        registers[c] = registers[a] + b;
        break;

    case Opcode::mulr:
        registers[c] = registers[a] * registers[b];
        break;

    case Opcode::muli:
        registers[c] = registers[a] * b;
        break;

    case Opcode::banr:
        registers[c] = registers[a] & registers[b];
        break;

    case Opcode::bani:
        registers[c] = registers[a] & b;
        break;

    case Opcode::borr:
        registers[c] = registers[a] | registers[b];
        break;

    case Opcode::bori:
        registers[c] = registers[a] | b;
        break;

    case Opcode::setr:
        registers[c] = registers[a];
        break;

    case Opcode::seti:
        registers[c] = a;
        break;

    case Opcode::gtir:
        registers[c] = a > registers[b];
        break;

    case Opcode::gtri:
        registers[c] = registers[a] > b;
        break;

    case Opcode::gtrr:
        registers[c] = registers[a] > registers[b];
        break;

    case Opcode::eqir:
        registers[c] = a == registers[b];
        break;

    case Opcode::eqri:
        registers[c] = registers[a] == b;
        break;

    case Opcode::eqrr:
        registers[c] = registers[a] == registers[b];
        break;
    }
    return registers;
}

bool is_valid_opcode(const Sample &sample, const Opcode opcode)
{
    const Row result = evaluate(sample[0], opcode, sample[1][1], sample[1][2], sample[1][3]);
    return result == sample[2];
}

bool behaves_like_three_opcodes(const Sample &sample)
{
    int count = 0;
    for (U16 opcode = 0; opcode < 16; ++opcode)
    {
        if (is_valid_opcode(sample, static_cast<Opcode>(opcode)))
        {
            ++count;
            if (count == 3)
            {
                return true;
            }
        }
    }

    return false;
}

S64 part_one(const Input &input)
{
    S64 count = 0;
    for (const Sample &sample : input.samples)
    {
        if (behaves_like_three_opcodes(sample))
        {
            ++count;
        }
    }
    return count;
}

std::array<Opcode, num_opcodes> determine_opcode_mapping(const std::vector<Sample> &samples)
{
    std::array<std::bitset<num_opcodes>, num_opcodes> possible_mappings;
    for (auto &bits : possible_mappings)
    {
        bits.set(); // all opcodes are possible initially
    }
    for (const Sample &sample : samples)
    {
        const U16 opcode_number = sample[1][0];
        for (U16 opcode = 0; opcode < num_opcodes; ++opcode)
        {
            if (!is_valid_opcode(sample, static_cast<Opcode>(opcode)))
            {
                possible_mappings[opcode_number].reset(opcode);
            }
        }
    }
    std::array<Opcode, num_opcodes> opcode_mapping{};
    std::bitset<num_opcodes> assigned_opcodes;
    while (assigned_opcodes.count() < num_opcodes)
    {
        for (U16 opcode_number = 0; opcode_number < num_opcodes; ++opcode_number)
        {
            if (possible_mappings[opcode_number].count() == 1)
            {
                const auto possible_mapping = possible_mappings[opcode_number];
                const auto as_ulong = possible_mapping.to_ulong();
                const auto opcode = static_cast<U16>(std::countr_zero(as_ulong));
                opcode_mapping[opcode_number] = static_cast<Opcode>(opcode);
                assigned_opcodes.set(opcode);
                // Remove this opcode from all other possible mappings
                for (U16 other_opcode_number = 0; other_opcode_number < num_opcodes; ++other_opcode_number)
                {
                    if (other_opcode_number != opcode_number)
                    {
                        possible_mappings[other_opcode_number].reset(opcode);
                    }
                }
            }
        }
    }
    return opcode_mapping;
}

S64 part_two(const Input &input)
{
    Row registers{};
    const auto opcode_mapping = determine_opcode_mapping(input.samples);
    for (const Row &instruction : input.instructions)
    {
        const Opcode opcode = opcode_mapping[instruction[0]];
        registers = evaluate(registers, opcode, instruction[1], instruction[2], instruction[3]);
    }
    return registers[0];
}
} // namespace

TEST_CASE("aoc example")
{
    const Sample sample{Row{3, 2, 1, 1}, Row{9, 2, 1, 2}, Row{3, 2, 2, 1}};

    REQUIRE(behaves_like_three_opcodes(sample));
}

SOLVE
{
    const Input input = read_input(std::cin);
    std::cout << part_one(input) << '\n';
    std::cout << part_two(input) << '\n';
}