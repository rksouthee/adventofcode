#include "aoc.h"
#include "aoc/vector.h"

#include <array>
#include <iostream>
#include <map>
#include <queue>
#include <span>
#include <string>
#include <vector>
#include <catch2/catch_test_macros.hpp>

using aoc::Vector2;

namespace
{
	std::vector<std::string> read_input(std::istream& input)
	{
		std::vector<std::string> result;
		for (std::string line; std::getline(input, line);)
		{
			result.push_back(std::move(line));
		}
		return result;
	}

	S64 get_numeric_part(const std::string_view code)
	{
		S64 index = 0;
		while (index < std::ssize(code) && code[index] == '0')
		{
			++index;
		}
		S64 numeric_code = 0;
		while (index < std::ssize(code) && std::isdigit(code[index]))
		{
			numeric_code = numeric_code * 10 + code[index] - '0';
			++index;
		}
		return numeric_code;
	}

	std::array<std::array<std::vector<std::string>, 11>, 11>& get_possible_paths()
	{
		static bool initialized = false;
		static std::array<std::array<std::vector<std::string>, 11>, 11> possible_paths;
		if (initialized) return possible_paths;
		possible_paths[0][0] = { "" };
		possible_paths[0][1] = { "^<" };
		possible_paths[0][2] = { "^" };
		possible_paths[0][3] = { "^>", ">^" };
		possible_paths[0][4] = { "^^<" };
		possible_paths[0][5] = { "^^" };
		possible_paths[0][6] = { "^^>", ">^^" };
		possible_paths[0][7] = { "^^^<" };
		possible_paths[0][8] = { "^^^" };
		possible_paths[0][9] = { "^^^>", ">^^^" };
		possible_paths[0][10] = { ">" };
		possible_paths[1][0] = { ">v" };
		possible_paths[1][1] = { "" };
		possible_paths[1][2] = { ">" };
		possible_paths[1][3] = { ">>" };
		possible_paths[1][4] = { "^" };
		possible_paths[1][5] = { "^>", ">^" };
		possible_paths[1][6] = { "^>>", ">>^" };
		possible_paths[1][7] = { "^^" };
		possible_paths[1][8] = { "^^>", ">^^" };
		possible_paths[1][9] = { "^^>>", ">>^^" };
		possible_paths[1][10] = { ">>v" };
		possible_paths[2][0] = { "v" };
		possible_paths[2][1] = { "<" };
		possible_paths[2][2] = { "" };
		possible_paths[2][3] = { ">" };
		possible_paths[2][4] = { "<^", "^<" };
		possible_paths[2][5] = { "^" };
		possible_paths[2][6] = { "^>", ">^" };
		possible_paths[2][7] = { "<^^", "^^<" };
		possible_paths[2][8] = { "^^" };
		possible_paths[2][9] = { "^^>", ">^^" };
		possible_paths[2][10] = { "v>", ">v" };
		possible_paths[3][0] = { "v<", "<v" };
		possible_paths[3][1] = { "<<" };
		possible_paths[3][2] = { "<" };
		possible_paths[3][3] = { "" };
		possible_paths[3][4] = { "<<^", "^<<" };
		possible_paths[3][5] = { "^<", "<^" };
		possible_paths[3][6] = { "^" };
		possible_paths[3][7] = { "<<^^", "^^<<" };
		possible_paths[3][8] = { "^^<", "<^^" };
		possible_paths[3][9] = { "^^", "^^" };
		possible_paths[3][10] = { "v" };
		possible_paths[4][0] = { ">vv" };
		possible_paths[4][1] = { "v" };
		possible_paths[4][2] = { "v>", ">v" };
		possible_paths[4][3] = { ">>v", "v>>" };
		possible_paths[4][4] = { "" };
		possible_paths[4][5] = { ">" };
		possible_paths[4][6] = { ">>" };
		possible_paths[4][7] = { "^" };
		possible_paths[4][8] = { "^>", ">^" };
		possible_paths[4][9] = { ">>^", "^>>" };
		possible_paths[4][10] = { ">>vv" };
		possible_paths[5][0] = { "vv" };
		possible_paths[5][1] = { "<v", "v<" };
		possible_paths[5][2] = { "v" };
		possible_paths[5][3] = { "v>", ">v" };
		possible_paths[5][4] = { "<" };
		possible_paths[5][5] = { "" };
		possible_paths[5][6] = { ">" };
		possible_paths[5][7] = { "<^", "^<" };
		possible_paths[5][8] = { "^" };
		possible_paths[5][9] = { "^>", ">^" };
		possible_paths[5][10] = { "vv>", ">vv" };
		possible_paths[6][0] = { "vv<", "<vv" };
		possible_paths[6][1] = { "<<v", "v<<" };
		possible_paths[6][2] = { "<v", "v<" };
		possible_paths[6][3] = { "v" };
		possible_paths[6][4] = { "<<" };
		possible_paths[6][5] = { "<" };
		possible_paths[6][6] = { "" };
		possible_paths[6][7] = { "<^^", "^^<" };
		possible_paths[6][8] = { "^<", "<^" };
		possible_paths[6][9] = { "^" };
		possible_paths[6][10] = { "vv" };
		possible_paths[7][0] = { ">vvv" };
		possible_paths[7][1] = { "vv" };
		possible_paths[7][2] = { "vv>", ">vv" };
		possible_paths[7][3] = { "vvv>", ">vvv" };
		possible_paths[7][4] = { "v" };
		possible_paths[7][5] = { "v>", ">v" };
		possible_paths[7][6] = { ">>v", "v>>" };
		possible_paths[7][7] = { "" };
		possible_paths[7][8] = { ">" };
		possible_paths[7][9] = { ">>" };
		possible_paths[7][10] = { ">>vvv" };
		possible_paths[8][0] = { "vvv" };
		possible_paths[8][1] = { "<vv", "vv<" };
		possible_paths[8][2] = { "vv" };
		possible_paths[8][3] = { "vv>", ">vv" };
		possible_paths[8][4] = { "<v", "v<" };
		possible_paths[8][5] = { "v" };
		possible_paths[8][6] = { "v>", ">v" };
		possible_paths[8][7] = { "<" };
		possible_paths[8][8] = { "" };
		possible_paths[8][9] = { ">" };
		possible_paths[8][10] = { "vvv>", ">vvv" };
		possible_paths[9][0] = { "vvv<", "<vvv" };
		possible_paths[9][1] = { "<<vv", "vv<<" };
		possible_paths[9][2] = { "<vv", "vv<" };
		possible_paths[9][3] = { "vv" };
		possible_paths[9][4] = { "<<v", "v<<" };
		possible_paths[9][5] = { "<v", "v<" };
		possible_paths[9][6] = { "v" };
		possible_paths[9][7] = { "<<" };
		possible_paths[9][8] = { "<" };
		possible_paths[9][9] = { "" };
		possible_paths[9][10] = { "vvv" };
		possible_paths[10][0] = { "<" };
		possible_paths[10][1] = { "^<<" };
		possible_paths[10][2] = { "<^", "^<" };
		possible_paths[10][3] = { "^" };
		possible_paths[10][4] = { "^^<<" };
		possible_paths[10][5] = { "^^<", "<^^" };
		possible_paths[10][6] = { "^^" };
		possible_paths[10][7] = { "^^^<<" };
		possible_paths[10][8] = { "^^^<", "<^^^" };
		possible_paths[10][9] = { "^^^" };
		possible_paths[10][10] = { "" };
		initialized = true;
		return possible_paths;
	}

	std::span<const std::string> get_possible_paths(const S64 source, const S64 destination)
	{
		return get_possible_paths()[source][destination];
	}

	S64 calculate_complexity(const std::string_view code, const S64 length)
	{
		return get_numeric_part(code) * length;
	}

	S64 from_hex(const char ch)
	{
		if (std::isdigit(ch))
		{
			return ch - '0';
		}
		return ch - 'A' + 10;
	}

	S64 from_position(const char ch)
	{
		switch (ch)
		{
		case 'A': return 0;
		case 'v': return 1;
		case '^': return 2;
		case '<': return 3;
		case '>': return 4;
		}
		return -1;
	}

	const std::array<std::array<std::vector<std::string_view>, 5>, 5>& get_move_map()
	{
		static bool initialized = false;
		static std::array<std::array<std::vector<std::string_view>, 5>, 5> result;
		if (initialized) return result;
		/* A -> A */ result[0][0] = { "A" };
		/* A -> v */ result[0][1] = { "<vA", "v<A" };
		/* A -> ^ */ result[0][2] = { "<A" };
		/* A -> < */ result[0][3] = { "v<<A" };
		/* A -> > */ result[0][4] = { "vA" };
		/* v -> A */ result[1][0] = { "^>A", ">^A" };
		/* v -> v */ result[1][1] = { "A" };
		/* v -> ^ */ result[1][2] = { "^A" };
		/* v -> < */ result[1][3] = { "<A" };
		/* v -> > */ result[1][4] = { ">A" };
		/* ^ -> A */ result[2][0] = { ">A" };
		/* ^ -> v */ result[2][1] = { "vA" };
		/* ^ -> ^ */ result[2][2] = { "A" };
		/* ^ -> < */ result[2][3] = { "v<A" };
		/* ^ -> > */ result[2][4] = { ">vA", "v>A" };
		/* < -> A */ result[3][0] = { ">>^A" };
		/* < -> v */ result[3][1] = { ">A" };
		/* < -> ^ */ result[3][2] = { ">^A" };
		/* < -> < */ result[3][3] = { "A" };
		/* < -> > */ result[3][4] = { ">>A" };
		/* > -> A */ result[4][0] = { "^A" };
		/* > -> v */ result[4][1] = { "<A" };
		/* > -> ^ */ result[4][2] = { "<^A", "^<A" };
		/* > -> < */ result[4][3] = { "<<A" };
		/* > -> > */ result[4][4] = { "A" };
		initialized = true;
		return result;
	}

	std::vector<std::string_view> get(const char current, const char expected)
	{
		return get_move_map()[from_position(current)][from_position(expected)];
	}

	S64 cost_of_moving(const char current, const char expected, const S64 depth)
	{
		if (depth == 0) return 1;
		S64 result = 0;
		char this_char = 'A';
		const auto& paths = get(current, expected);
		for (const char ch : paths.front())
		{
			result += cost_of_moving(this_char, ch, depth - 1);
			this_char = ch;
		}
		return result;
	}

	S64 cost_of_moving(const std::string_view moves)
	{
		S64 result = 0;
		char current = 'A';
		for (const char ch : moves)
		{
			result += cost_of_moving(current, ch, 2);
			current = ch;
		}
		return result;
	}

	S64 get_shortest_path(const std::string_view code)
	{
		std::vector<S64> lengths = { 0 };
		char current = 'A';
		for (const char ch : code)
		{
			std::vector<S64> new_lengths;
			for (const S64 length : lengths)
			{
				const std::span<const std::string> paths = get_possible_paths(from_hex(current), from_hex(ch));
				assert(!paths.empty());
				for (const std::string& path : paths)
				{
					new_lengths.emplace_back(length + cost_of_moving(path + "A"));
				}
			}
			current = ch;
			lengths = std::move(new_lengths);
		}
		assert(current == 'A');
		return std::ranges::min(lengths);
	}

	S64 part_one(const std::vector<std::string>& codes)
	{
		S64 sum = 0;
		for (const std::string& code : codes)
		{
			const S64 length = get_shortest_path(code);
			sum += calculate_complexity(code, length);
		}
		return sum;
	}
}

TEST_CASE("2024 21 1", "[2024][21][1]")
{
	REQUIRE(cost_of_moving('A', '<', 2) == 10);
	REQUIRE(cost_of_moving('<', 'A', 2) == 8);

	REQUIRE(get_shortest_path("029A") == 68);
	REQUIRE(get_shortest_path("980A") == 60);
	REQUIRE(get_shortest_path("179A") == 68);
	REQUIRE(get_shortest_path("456A") == 64);
	REQUIRE(get_shortest_path("379A") == 64);

	REQUIRE(part_one({ "029A", "980A", "179A", "456A", "379A" }) == 126384);
}

SOLVE
{
	const std::vector<std::string> codes = read_input(std::cin);

	std::cout << part_one(codes) << '\n';
	//std::cout << part_two(codes) << '\n';
}
