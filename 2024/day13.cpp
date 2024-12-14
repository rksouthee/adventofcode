#include <cassert>

#include "aoc.h"
#include "aoc/string_helpers.h"
#include "aoc/vector.h"

#include <iostream>
#include <regex>

using aoc::Vector2;

namespace
{
	struct Machine
	{
		Vector2 button_a;
		Vector2 button_b;
		Vector2 prize;
	};

	Vector2 parse_vector2(const std::string& line, const std::regex& regex)
	{
		std::smatch match;
		const bool result = std::regex_match(line, match, regex);
		assert(result);
		return { aoc::convert_unguarded<S64>(match[1].str()), aoc::convert_unguarded<S64>(match[2].str()) };
	}

	Vector2 parse_button(const std::string& line)
	{
		static const std::regex regex(R"(Button [AB]: X\+(\d+), Y\+(\d+))");
		return parse_vector2(line, regex);
	}

	Vector2 parse_prize(const std::string& line)
	{
		static const std::regex regex(R"(Prize: X=(\d+), Y=(\d+))");
		return parse_vector2(line, regex);
	}

	std::vector<Machine> read_input(std::istream& input)
	{
		std::vector<std::string> lines;
		std::string line;
		while (std::getline(input, line))
		{
			if (!line.empty())
			{
				lines.push_back(line);
			}
		}

		std::vector<Machine> machines;
		for (S64 i = 0; i < std::ssize(lines); i += 3)
		{
			machines.emplace_back(parse_button(lines[i]), parse_button(lines[i + 1]), parse_prize(lines[i + 2]));
		}

		return machines;
	}

	Vector2 find_solution(const Machine& machine)
	{
		const S64 determinant = machine.button_a.x * machine.button_b.y - machine.button_a.y * machine.button_b.x;
		if (determinant == 0)
		{
			return { 0, 0 };
		}

		const S64 x = machine.button_b.y * machine.prize.x - machine.button_b.x * machine.prize.y;
		if (x % determinant != 0)
		{
			return { 0, 0 };
		}

		const S64 y = machine.button_a.x * machine.prize.y - machine.button_a.y * machine.prize.x;
		if (y % determinant != 0)
		{
			return { 0, 0 };
		}

		return { x / determinant, y / determinant };
	}

	S64 calculate_tokens_required(const Machine& machine)
	{
		const Vector2 presses = find_solution(machine);
		return presses.x * 3 + presses.y;
	}

	S64 part_one(const std::vector<Machine>& machines)
	{
		S64 tokens = 0;
		for (const Machine& machine : machines)
		{
			tokens += calculate_tokens_required(machine);
		}
		return tokens;
	}

	S64 part_two(const std::vector<Machine>& machines)
	{
		S64 tokens = 0;
		for (Machine machine : machines)
		{
			constexpr S64 offset = 10000000000000;
			machine.prize.x += offset;
			machine.prize.y += offset;
			tokens += calculate_tokens_required(machine);
		}
		return tokens;
	}
}

SOLVE
{
	const std::vector<Machine> machines = read_input(std::cin);
	std::cout << part_one(machines) << '\n';
	std::cout << part_two(machines) << '\n';
}