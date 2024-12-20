#include "aoc.h"
#include "aoc/core.h"
#include "aoc/string_helpers.h"

#include <array>
#include <iostream>
#include <regex>
#include <span>
#include <sstream>
#include <string>
#include <vector>

namespace
{
	struct Computer
	{
		std::array<S64, 3> registers;
		std::span<const U8> program;
		S64 ip;
		std::vector<S64> output;
	};

	S64 parse_register(const std::string& line)
	{
		static const std::regex regex(R"(Register [ABC]: (\d+))");
		std::smatch match;
		std::regex_match(line, match, regex);
		return std::stoll(match[1]);
	}

	Computer read_computer(std::istream& input)
	{
		Computer computer{};
		std::string line;
		for (S64& reg : computer.registers)
		{
			std::getline(input, line);
			reg = parse_register(line);
		}
		return computer;
	}

	std::vector<U8> read_program(std::istream& input)
	{
		std::string line;
		while (std::getline(input, line))
		{
			if (!line.empty()) break;
		}
		line = line.substr(9); // Skip "Program: "
		const std::vector<std::string_view> tokens = aoc::split(line, ',');
		std::vector<U8> program;
		program.reserve(tokens.size());
		std::ranges::transform(tokens, std::back_inserter(program), aoc::convert_unguarded<U8>);
		return program;
	}

	bool should_halt(const Computer& computer)
	{
		return !(computer.ip >= 0 && computer.ip < std::ssize(computer.program));
	}

	S64 get_combo_operand(const Computer& computer, const S64 operand)
	{
		switch (operand)
		{
		case 0:
		case 1:
		case 2:
		case 3:
			return operand;
		case 4:
		case 5:
		case 6:
			return computer.registers[operand - 4];
		case 7:
		default:
			break;
		}
		return 0;
	}

	// @todo: Move this into shared library
	S64 power(const S64 base, const S64 exponent)
	{
		S64 result = 1;
		for (S64 i = 0; i < exponent; ++i)
		{
			result *= base;
		}
		return result;
	}

	S64 divide_op(const Computer& computer, const S64 operand)
	{
		const S64 numerator = computer.registers[0];
		const S64 denominator = power(2, get_combo_operand(computer, operand));
		const S64 result = numerator / denominator;
		return result;
	}

	void execute(Computer& computer)
	{
		const S64 opcode = computer.program[computer.ip];
		const S64 operand = computer.program[computer.ip + 1];
		computer.ip += 2;

		switch (opcode)
		{
		case 0:
		{
			computer.registers[0] = divide_op(computer, operand);
			break;
		}
		case 1:
		{
			const S64 lhs = computer.registers[1];
			const S64 rhs = operand;
			const S64 result = lhs ^ rhs;
			computer.registers[1] = result;
			break;
		}
		case 2:
		{
			const S64 value = get_combo_operand(computer, operand);
			const S64 result = value % 8;
			computer.registers[1] = result;
			break;
		}
		case 3:
		{
			if (computer.registers[0] != 0)
			{
				computer.ip = operand;
			}
			break;
		}
		case 4:
		{
			const S64 b = computer.registers[1];
			const S64 c = computer.registers[2];
			const S64 result = b ^ c;
			computer.registers[1] = result;
			break;
		}
		case 5:
		{
			const S64 value = get_combo_operand(computer, operand);
			const S64 result = value % 8;
			computer.output.push_back(result);
			break;
		}
		case 6:
		{
			computer.registers[1] = divide_op(computer, operand);
			break;
		}
		case 7:
		{
			computer.registers[2] = divide_op(computer, operand);
			break;
		}
		default:
			break;
		}
	}

	// @todo: Could be generic, and moved into shared library
	std::string join(const std::vector<S64>& values, const char separator)
	{
		if (values.empty()) return "";

		std::ostringstream oss;
		oss << values.front();
		for (S64 i = 1; i < std::ssize(values); ++i)
		{
			oss << separator << values[i];
		}
		return oss.str();
	}

	std::string part_one(Computer computer)
	{
		while (!should_halt(computer))
		{
			execute(computer);
		}
		return join(computer.output, ',');
	}

	void run_loop_once(Computer& computer)
	{
		computer.ip = 0;
		do
		{
			execute(computer);
		} while (computer.ip > 0 && computer.ip < std::ssize(computer.program));
	}

	S64 part_two(const Computer& computer)
	{
		const S64 program_size = std::ssize(computer.program);
		std::vector<S64> values = { 0 };
		for (S64 i = 0; i < program_size; ++i)
		{
			std::vector<S64> new_values;
			const S64 expected_digit = computer.program[program_size - i - 1];
			for (const S64 a : values)
			{
				for (S64 j = 0; j < 8; ++j)
				{
					Computer copy = computer;
					const S64 new_a = a * 8 + j;
					copy.registers[0] = new_a;
					run_loop_once(copy);
					if (copy.output[0] == expected_digit)
					{
						new_values.push_back(new_a);
					}
				}
			}
			values = std::move(new_values);
		}

		if (values.empty()) return 0;
		return std::ranges::min(values);
	}
}

SOLVE
{
	Computer computer = read_computer(std::cin);
	const std::vector<U8> program = read_program(std::cin);
	computer.program = program;

	std::cout << part_one(computer) << '\n';
	std::cout << part_two(computer) << '\n';
}