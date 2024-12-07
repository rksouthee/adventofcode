#include "aoc.h"
#include "aoc/core.h"
#include "aoc/string_helpers.h"

#include <algorithm>
#include <iostream>
#include <span>
#include <string>
#include <vector>

namespace
{
	struct Equation
	{
		U64 target;
		std::vector<U64> operands;
	};

	Equation parse_equation(const std::string_view str)
	{
		Equation equation;
		const std::vector<std::string_view> parts = aoc::split(str, ": ");
		equation.target = aoc::convert_unguarded<U64>(parts[0]);
		const std::vector<std::string_view> operands = aoc::split(parts[1], ' ');
		equation.operands.reserve(operands.size());
		std::ranges::transform(operands, std::back_inserter(equation.operands), aoc::convert_unguarded<U64>);
		return equation;
	}

	std::vector<Equation> read_input(std::istream& input)
	{
		std::vector<Equation> equations;
		std::string line;
		while (std::getline(input, line))
		{
			equations.push_back(parse_equation(line));
		}
		return equations;
	}

	struct Operation
	{
		bool (*predicate)(U64, U64);
		U64 (*operation)(U64, U64);
	};

	bool can_divide(const U64 a, const U64 b)
	{
		return a % b == 0;
	}

	U64 divide(const U64 a, const U64 b)
	{
		return a / b;
	}

	bool can_subtract(const U64 a, const U64 b)
	{
		return a >= b;
	}

	U64 subtract(const U64 a, const U64 b)
	{
		return a - b;
	}

	bool can_split(const U64 a, const U64 b)
	{
		const std::string a_str = std::to_string(a);
		const std::string b_str = std::to_string(b);
		return a_str.ends_with(b_str);
	}

	U64 split(const U64 a, const U64 b)
	{
		std::string a_str = std::to_string(a);
		const std::string b_str = std::to_string(b);
		a_str.erase(a_str.size() - b_str.size());
		return aoc::convert_unguarded<U64>(a_str);
	}

	bool can_solve(const U64 target, std::span<const U64> operands, const std::span<const Operation> operations)
	{
		if (operands.empty())
		{
			return target == 0;
		}

		if (operands.size() == 1)
		{
			return target == operands[0];
		}

		const U64 operand = operands.back();
		operands = operands.subspan(0, operands.size() - 1);
		return std::ranges::any_of(operations, [target, operand, &operands, &operations](const Operation& op)
			{
				return op.predicate(target, operand) && can_solve(op.operation(target, operand), operands, operations);
			});
	}

	const std::vector<Operation> s_operations =
	{
		{.predicate= can_divide, .operation= divide},
		{.predicate= can_subtract, .operation= subtract},
		{.predicate= can_split, .operation= split}
	};

	U64 solve(const std::vector<Equation>& equations, const std::span<const Operation> operations)
	{
		U64 result = 0;
		for (const auto& [target, operands] : equations)
		{
			if (can_solve(target, operands, operations))
			{
				result += target;
			}
		}
		return result;
	}
}

SOLVE
{
	const std::vector<Equation> equations = read_input(std::cin);

	std::cout << solve(equations, std::span{ s_operations.begin(), 2 }) << '\n';
	std::cout << solve(equations, s_operations) << '\n';
}