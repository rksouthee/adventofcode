#include "aoc.h"
#include "aoc/core.h"
#include "aoc/vector.h"

#include <array>
#include <iostream>
#include <set>
#include <span>
#include <string>

using aoc::Vector2;

namespace
{
	std::string read_input(std::istream& input)
	{
		std::string result;
		input >> result;
		return result;
	}

	Vector2 vector2_from_char(const char direction)
	{
		switch (direction)
		{
		case '^': return Vector2::north;
		case 'v': return Vector2::south;
		case '<': return Vector2::west;
		case '>': return Vector2::east;
		default: return {};
		}
	}

	S64 deliver(const std::span<Vector2> santas, const std::string_view input)
	{
		std::set visited(santas.begin(), santas.end());
		for (S64 i = 0; i < std::ssize(input); ++i)
		{
			Vector2& position = santas[i % santas.size()];
			position += vector2_from_char(input[i]);
			visited.insert(position);
		}
		return std::ssize(visited);
	}

	S64 part_one(const std::string_view input)
	{
		Vector2 start{};
		return deliver(std::span{ &start, 1 }, input);
	}

	S64 part_two(const std::string_view input)
	{
		constexpr Vector2 start{};
		std::array santas = { start, start };
		return deliver(santas, input);
	}
}

SOLVE
{
	const std::string input = read_input(std::cin);

	std::cout << part_one(input) << '\n';
	std::cout << part_two(input) << '\n';
}