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
		static std::array<std::array<std::vector<std::string>, 11>, 11> possible_paths;
		possible_paths[0][0] = { };
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
		possible_paths[1][1] = { };
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
		possible_paths[2][2] = { };
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
		possible_paths[3][3] = { };
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
		possible_paths[4][4] = {};
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
		possible_paths[5][5] = { };
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
		possible_paths[6][6] = { };
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
		possible_paths[7][7] = { };
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
		possible_paths[8][8] = { };
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
		possible_paths[9][9] = { };
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
		possible_paths[10][10] = { };
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

	std::map<std::tuple<char, std::string, std::size_t>, S64> cost_cache;

	S64 cost_of_moving(const char target, std::string& positions, const std::size_t robots)
	{
		const auto key = std::tuple{ target, positions, robots };
		if (const auto it = cost_cache.find(key); it != cost_cache.end())
		{
			return it->second;
		}

		S64 cost = 0;
		if (robots == 0)
		{
			cost = 1;
		}
		else if (const char source = positions[robots - 1]; source == target)
		{
			cost = cost_of_moving('A', positions, robots - 1);
		}
		else
		{
			switch (source)
			{
			case '^':
			{
				switch (target)
				{
				case 'A':
				{
					cost = cost_of_moving('>', positions, robots - 1) + cost_of_moving('A', positions, robots - 1);
					break;
				}
				case '<':
				{
					cost = cost_of_moving('v', positions, robots - 1) + cost_of_moving('<', positions, robots - 1) + cost_of_moving('A', positions, robots - 1);
					break;
				}
				case 'v':
				{
					cost = cost_of_moving('v', positions, robots - 1) + cost_of_moving('A', positions, robots - 1);
					break;
				}
				case '>':
				{
					std::string temp = positions;
					const S64 cost1 = cost_of_moving('v', positions, robots - 1) + cost_of_moving('>', positions, robots - 1) + cost_of_moving('A', positions, robots - 1);
					std::swap(positions, temp);
					const S64 cost2 = cost_of_moving('>', positions, robots - 1) + cost_of_moving('v', positions, robots - 1) + cost_of_moving('A', positions, robots - 1);
					if (cost1 < cost2)
					{
						positions = std::move(temp);
						cost = cost1;
					}
					else
					{
						cost = cost2;
					}
				}
				break;
				}
				break;
			}
			case 'A':
			{
				switch (target)
				{
				case '^':
				{
					cost = cost_of_moving('<', positions, robots - 1) + cost_of_moving('A', positions, robots - 1);
					break;
				}
				case '<':
				{
					cost = cost_of_moving('v', positions, robots - 1) + cost_of_moving('<', positions, robots - 1) + cost_of_moving('<', positions, robots - 1) + cost_of_moving('A', positions, robots - 1);
					break;
				}
				case 'v':
				{
					std::string temp = positions;
					const S64 cost1 = cost_of_moving('v', positions, robots - 1) + cost_of_moving('<', positions, robots - 1) + cost_of_moving('A', positions, robots - 1);
					std::swap(positions, temp);
					const S64 cost2 = cost_of_moving('<', positions, robots - 1) + cost_of_moving('v', positions, robots - 1) + cost_of_moving('A', positions, robots - 1);
					if (cost1 < cost2)
					{
						positions = std::move(temp);
						cost = cost1;
					}
					else
					{
						cost = cost2;
					}
					break;
				}
				case '>':
				{
					cost = cost_of_moving('v', positions, robots - 1) + cost_of_moving('A', positions, robots - 1);
					break;
				}
				}
				break;
			}
			case '<':
			{
				switch (target)
				{
				case '^':
				{
					cost = cost_of_moving('>', positions, robots - 1) + cost_of_moving('^', positions, robots - 1) + cost_of_moving('A', positions, robots - 1);
					break;
				}
				case 'A':
				{
					cost = cost_of_moving('>', positions, robots - 1) + cost_of_moving('>', positions, robots - 1) + cost_of_moving('^', positions, robots - 1) + cost_of_moving('A', positions, robots - 1);
					break;
				}
				case 'v':
				{
					cost = cost_of_moving('>', positions, robots - 1) + cost_of_moving('A', positions, robots - 1);
					break;
				}
				case '>':
				{
					cost = cost_of_moving('>', positions, robots - 1) + cost_of_moving('>', positions, robots - 1) + cost_of_moving('A', positions, robots - 1);
					break;
				}
				}
				break;
			}
			case 'v':
			{
				switch (target)
				{
				case '^':
				{
					cost = cost_of_moving('^', positions, robots - 1) + cost_of_moving('A', positions, robots - 1);
					break;
				}
				case 'A':
				{
					std::string temp = positions;
					const S64 cost1 = cost_of_moving('^', positions, robots - 1) + cost_of_moving('>', positions, robots - 1) + cost_of_moving('A', positions, robots - 1);
					std::swap(temp, positions);
					const S64 cost2 = cost_of_moving('>', positions, robots - 1) + cost_of_moving('^', positions, robots - 1) + cost_of_moving('A', positions, robots - 1);
					if (cost1 < cost2)
					{
						positions = std::move(temp);
						cost = cost1;
					}
					else
					{
						cost = cost2;
					}
					break;
				}
				case '<':
				{
					cost = cost_of_moving('<', positions, robots - 1) + cost_of_moving('A', positions, robots - 1);
					break;
				}
				case '>':
				{
					cost = cost_of_moving('>', positions, robots - 1) + cost_of_moving('A', positions, robots - 1);
					break;
				}
				}
				break;
			}
			case '>':
			{
				switch (target)
				{
				case '^':
				{
					std::string temp = positions;
					const S64 cost1 = cost_of_moving('^', positions, robots - 1) + cost_of_moving('<', positions, robots - 1) + cost_of_moving('A', positions, robots - 1);
					std::swap(temp, positions);
					const S64 cost2 = cost_of_moving('<', positions, robots - 1) + cost_of_moving('^', positions, robots - 1) + cost_of_moving('A', positions, robots - 1);
					if (cost1 < cost2)
					{
						positions = std::move(temp);
						cost = cost1;
					}
					else
					{
						cost = cost2;
					}
					break;
				}
				case 'A':
				{
					cost = cost_of_moving('^', positions, robots - 1) + cost_of_moving('A', positions, robots - 1);
					break;
				}
				case '<':
				{
					cost = cost_of_moving('<', positions, robots - 1) + cost_of_moving('<', positions, robots - 1) + cost_of_moving('A', positions, robots - 1);
					break;
				}
				case 'v':
				{
					cost = cost_of_moving('<', positions, robots - 1) + cost_of_moving('A', positions, robots - 1);
					break;
				}
				}
				break;
			}
			}
		}
		if (robots > 0)
		{
			positions[robots - 1] = target;
		}
		cost_cache[key] = cost;
		return cost;
	}

	S64 get_shortest_path(const char source, const std::string_view code, const S64 current_length, S64 best, const std::string& robots)
	{
		if (code.empty())
		{
			return current_length;
		}

		const auto possible_paths = get_possible_paths(from_hex(source), from_hex(code.front()));
		for (const std::string& path : possible_paths)
		{
			std::string copy = robots;
			S64 this_path = current_length;
			for (const char ch : path)
			{
				this_path += cost_of_moving(ch, copy, copy.size());
			}
			this_path += cost_of_moving('A', copy, copy.size());
			this_path = get_shortest_path(code.front(), code.substr(1), this_path, best, copy);
			best = std::min(best, this_path);
		}
		return best;
	}

	S64 part_one(const std::vector<std::string>& codes)
	{
		S64 sum = 0;
		for (const std::string& code : codes)
		{
			const S64 length = get_shortest_path('A', code, 0, std::numeric_limits<S64>::max(), "AA");
			sum += calculate_complexity(code, length);
		}
		return sum;
	}

	S64 part_two(const std::vector<std::string>& codes)
	{
		const std::string robots(25, 'A');
		S64 sum = 0;
		for (const std::string& code : codes)
		{
			const S64 length = get_shortest_path('A', code, 0, std::numeric_limits<S64>::max(), robots);
			sum += calculate_complexity(code, length);
		}
		return sum;
	}
}

TEST_CASE("2024 21 1", "[2024][21][1]")
{
	std::string robots = "AA";
	REQUIRE(cost_of_moving('<', robots, robots.size()) == 10);
	REQUIRE(cost_of_moving('A', robots, robots.size()) == 8);
	robots = "AA";
	REQUIRE(cost_of_moving('<', robots, robots.size()) == 10);
}

SOLVE
{
	const std::vector<std::string> codes = read_input(std::cin);

	std::cout << part_one(codes) << '\n';
	//std::cout << part_two(codes) << '\n';
}
