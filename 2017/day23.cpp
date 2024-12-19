#include "aoc.h"
#include "aoc/core.h"

#include <array>
#include <iostream>
#include <regex>
#include <span>
#include <string>

#include <cassert>

namespace
{
	enum class Opcode : U8
	{
		set,
		sub,
		mul,
		jnz,
	};

	struct Instruction
	{
		Opcode opcode;
		std::string arg1;
		std::string arg2;
	};

	struct Machine
	{
		std::span<const Instruction> instructions;
		S64 ip = 0;
		std::array<S64, 8> registers;
	};

	Instruction parse_instruction(const std::string& line)
	{
		static const std::regex re{ R"((\w+) (.*) (.*))" };
		std::smatch match;
		std::regex_match(line, match, re);

		Instruction instruction;
		instruction.arg1 = match[2];
		instruction.arg2 = match[3];

		if (match[1] == "set") instruction.opcode = Opcode::set;
		else if (match[1] == "sub") instruction.opcode = Opcode::sub;
		else if (match[1] == "mul") instruction.opcode = Opcode::mul;
		else if (match[1] == "jnz") instruction.opcode = Opcode::jnz;
		else throw std::runtime_error("Invalid opcode");

		return instruction;
	}

	std::vector<Instruction> read_input(std::istream& input)
	{
		std::vector<Instruction> instructions;
		for (std::string line; std::getline(input, line);)
		{
			instructions.push_back(parse_instruction(line));
		}
		return instructions;
	}

	bool is_register(const std::string_view arg)
	{
		return arg.size() == 1 && arg[0] >= 'a' && arg[0] <= 'h';
	}

	S64& get_register(Machine& machine, const std::string& reg)
	{
		assert(is_register(reg));
		return machine.registers[reg[0] - 'a'];
	}

	S64 get_value(const Machine& machine, const std::string& arg)
	{
		if (is_register(arg))
		{
			return machine.registers[arg[0] - 'a'];
		}
		return std::stoll(arg);
	}

	void execute(Machine& machine)
	{
		if (machine.ip < 0 || machine.ip >= std::ssize(machine.instructions))
		{
			return;
		}

		const auto& [opcode, arg1, arg2] = machine.instructions[machine.ip];

		switch (opcode)
		{
		case Opcode::set:
		{
			get_register(machine, arg1) = get_value(machine, arg2);
			break;
		}
		case Opcode::sub:
		{
			get_register(machine, arg1) -= get_value(machine, arg2);
			break;
		}
		case Opcode::mul:
		{
			get_register(machine, arg1) *= get_value(machine, arg2);
			break;
		}
		case Opcode::jnz:
		{
			if (get_value(machine, arg1) != 0)
			{
				machine.ip += get_value(machine, arg2);
				return;
			}
			break;
		}
		}
		++machine.ip;
	}

	S64 part_one(const std::span<const Instruction> instructions)
	{
		Machine machine{};
		machine.instructions = instructions;

		S64 mul_count = 0;
		while (machine.ip >= 0 && machine.ip < std::ssize(machine.instructions))
		{
			if (machine.instructions[machine.ip].opcode == Opcode::mul)
			{
				++mul_count;
			}
			execute(machine);
		}
		return mul_count;
	}

	S64 smallest_divisor(const S64 n)
	{
		assert(n > 0);
		if (n % 2 == 0) return 2;
		for (S64 i = 3; i * i <= n; i += 2)
		{
			if (n % i == 0) return i;
		}
		return n;
	}

	// @todo: Move this into the core library.
	bool is_prime(const S64 n)
	{
		return n == smallest_divisor(n);
	}

	S64 part_two()
	{
		// The program calculates the number of non-prime numbers between b and c (inclusive) with a step of 17.
		constexpr S64 b = 109900;
		constexpr S64 c = 126900;
		S64 h = 0;
		S64 g = b;
		while (g <= c)
		{
			if (!is_prime(g)) ++h;
			g += 17;
		}
		return h;
	}

}

SOLVE
{
	const std::vector<Instruction> instructions = read_input(std::cin);

	std::cout << part_one(instructions) << '\n';
	std::cout << part_two() << '\n';
}