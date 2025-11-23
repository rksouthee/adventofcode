#include "aoc.h"

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

    struct Instruction
    {
        Opcode opcode;
        S64 a;
        S64 b;
        S64 c;
    };

    struct Device
    {
        std::array<S64, 6> registers;
        S64 ip_register;
        S64 ip_value;
        std::span<const Instruction> program;
    };

    std::pair<S64, std::vector<Instruction>> read_input(std::istream& is)
    {
        std::string line;
        std::getline(is, line);
        const S64 ip_register = std::stoi(line.substr(4));
        std::vector<Instruction> program;
        while (std::getline(is, line))
        {
            std::istringstream line_ss(line);
            std::string opcode_str;
            S64 a, b, c;
            line_ss >> opcode_str >> a >> b >> c;
            Opcode opcode;
            if (opcode_str == "addr") opcode = Opcode::addr;
            else if (opcode_str == "addi") opcode = Opcode::addi;
            else if (opcode_str == "mulr") opcode = Opcode::mulr;
            else if (opcode_str == "muli") opcode = Opcode::muli;
            else if (opcode_str == "banr") opcode = Opcode::banr;
            else if (opcode_str == "bani") opcode = Opcode::bani;
            else if (opcode_str == "borr") opcode = Opcode::borr;
            else if (opcode_str == "bori") opcode = Opcode::bori;
            else if (opcode_str == "setr") opcode = Opcode::setr;
            else if (opcode_str == "seti") opcode = Opcode::seti;
            else if (opcode_str == "gtir") opcode = Opcode::gtir;
            else if (opcode_str == "gtri") opcode = Opcode::gtri;
            else if (opcode_str == "gtrr") opcode = Opcode::gtrr;
            else if (opcode_str == "eqir") opcode = Opcode::eqir;
            else if (opcode_str == "eqri") opcode = Opcode::eqri;
            else if (opcode_str == "eqrr") opcode = Opcode::eqrr;
            program.emplace_back(opcode, a, b, c);
        }
        return { ip_register, std::move(program) };
    }

    bool evaluate(Device& device)
    {
        if (device.ip_value < 0 || device.ip_value >= std::ssize(device.program))
        {
            return false;
        }

        const Instruction& instruction = device.program[device.ip_value];
        device.registers[device.ip_register] = device.ip_value;

        switch (instruction.opcode)
        {
        case Opcode::addr:
            device.registers[instruction.c] = device.registers[instruction.a] + device.registers[instruction.b];
            break;
        case Opcode::addi:
            device.registers[instruction.c] = device.registers[instruction.a] + instruction.b;
            break;
        case Opcode::mulr:
            device.registers[instruction.c] = device.registers[instruction.a] * device.registers[instruction.b];
            break;
        case Opcode::muli:
            device.registers[instruction.c] = device.registers[instruction.a] * instruction.b;
            break;
        case Opcode::banr:
            device.registers[instruction.c] = device.registers[instruction.a] & device.registers[instruction.b];
            break;
        case Opcode::bani:
            device.registers[instruction.c] = device.registers[instruction.a] & instruction.b;
            break;
        case Opcode::borr:
            device.registers[instruction.c] = device.registers[instruction.a] | device.registers[instruction.b];
            break;
        case Opcode::bori:
            device.registers[instruction.c] = device.registers[instruction.a] | instruction.b;
            break;
        case Opcode::setr:
            device.registers[instruction.c] = device.registers[instruction.a];
            break;
        case Opcode::seti:
            device.registers[instruction.c] = instruction.a;
            break;
        case Opcode::gtir:
            device.registers[instruction.c] = (instruction.a > device.registers[instruction.b]) ? 1 : 0;
            break;
        case Opcode::gtri:
            device.registers[instruction.c] = (device.registers[instruction.a] > instruction.b) ? 1 : 0;
            break;
        case Opcode::gtrr:
            device.registers[instruction.c] = (device.registers[instruction.a] > device.registers[instruction.b]) ? 1 : 0;
            break;
        case Opcode::eqir:
            device.registers[instruction.c] = (instruction.a == device.registers[instruction.b]) ? 1 : 0;
            break;
        case Opcode::eqri:
            device.registers[instruction.c] = (device.registers[instruction.a] == instruction.b) ? 1 : 0;
            break;
        case Opcode::eqrr:
            device.registers[instruction.c] = (device.registers[instruction.a] == device.registers[instruction.b]) ? 1 : 0;
            break;
        default:
            break;
        }

        device.ip_value = device.registers[device.ip_register];
        ++device.ip_value;

        return true;
    }

    S64 part_one(const S64 ip_register, const std::span<const Instruction> program)
    {
        Device device{};
        device.ip_register = ip_register;
        device.program = program;

        while (evaluate(device))
        {
        }

        return device.registers[0];
    }

    S64 part_two(const S64 ip_register, const std::span<const Instruction> program)
    {
        // TODO: runs too long - need to analyze the program to find a better solution
        Device device{};
        device.ip_register = ip_register;
        device.program = program;
        device.registers[0] = 1;

        while (evaluate(device))
        {
        }

        return device.registers[0];
    }
}

TEST_CASE("aoc example")
{
    const std::array<Instruction, 7> program =
    {
        Instruction{ Opcode::seti, 5, 0, 1 },
        Instruction{ Opcode::seti, 6, 0, 2 },
        Instruction{ Opcode::addi, 0, 1, 0 },
        Instruction{ Opcode::addr, 1, 2, 3 },
        Instruction{ Opcode::setr, 1, 0, 0 },
        Instruction{ Opcode::seti, 8, 0, 4 },
        Instruction{ Opcode::seti, 9, 0, 5 },
    };
    Device device{};
    REQUIRE(evaluate(device));
    REQUIRE(device.registers == std::array<S64, 6>{0, 5, 0, 0, 0, 0});
    REQUIRE(device.ip_value == 1);

    REQUIRE(evaluate(device));
    REQUIRE(device.registers == std::array<S64, 6>{1, 5, 6, 0, 0, 0});
    REQUIRE(device.ip_value == 2);

    REQUIRE(evaluate(device));
    REQUIRE(device.registers == std::array<S64, 6>{3, 5, 6, 0, 0, 0});
    REQUIRE(device.ip_value == 4);

    REQUIRE(evaluate(device));
    REQUIRE(device.registers == std::array<S64, 6>{5, 5, 6, 0, 0, 0});
    REQUIRE(device.ip_value == 6);

    REQUIRE(evaluate(device));
    REQUIRE(device.registers == std::array<S64, 6>{6, 5, 6, 0, 0, 9});
    REQUIRE(device.ip_value == 7);

    REQUIRE(!evaluate(device));
}

SOLVE
{
    const std::pair<S64, std::vector<Instruction>> input = read_input(std::cin);
    std::cout << part_one(input.first, input.second) << '\n';
    //std::cout << part_two(input.first, input.second) << '\n';
}