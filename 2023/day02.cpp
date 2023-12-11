#include "aoc.h"
#include <aoc/algorithm.h>
#include <aoc/string_helpers.h>

#include <catch2/catch_test_macros.hpp>

#include <iostream>
#include <sstream>

#include <cassert>

namespace
{
	struct Cube
	{
		int count;
		std::string_view colour;
	};

	struct Subset
	{
		int red;
		int green;
		int blue;
	};

	using Game = std::vector<Subset>;

	Cube parse_cube(const std::string_view sv)
	{
		int count = 0;
		const auto end = aoc::parse_nonnegative_integer(sv.begin(), sv.end(), count);
		return {count, {end + 1, sv.end()}};
	}

	Subset parse_subset(const std::string_view sv)
	{
		Subset cubes{};
		for (const std::string_view& c_sv : aoc::split(sv, ", "))
		{
			const Cube cube = parse_cube(c_sv);
			if (cube.colour == "red")
			{
				cubes.red = cube.count;
			}
			else if (cube.colour == "green")
			{
				cubes.green = cube.count;
			}
			else if (cube.colour == "blue")
			{
				cubes.blue = cube.count;
			}
			else
			{
				std::cerr << "Invalid colour '" << cube.colour << '\'' << std::endl;
				std::abort();
			}
		}
		return cubes;
	}

	Game parse_game(const std::string_view sv)
	{
		Game game;
		for (const std::string_view& s_sv : aoc::split(sv, "; "))
		{
			game.push_back(parse_subset(s_sv));
		}
		return game;
	}

	std::vector<Game> parse_input(std::istream& is)
	{
		std::vector<Game> games;
		std::string line;
		while (std::getline(is, line))
		{
			auto colon_iter = std::find(line.begin(), line.end(), ':');
			assert(colon_iter != line.end());
			const std::string_view game_sv(colon_iter + 2, line.end());
			games.push_back(parse_game(game_sv));
		}
		return games;
	}

	bool is_subset_possible(const Subset& cubes, const Subset& colours)
	{
		return colours.red >= cubes.red && colours.green >= cubes.green && colours.blue >= cubes.blue;
	}

	bool is_game_possible(const Game& game, const Subset& colours)
	{
		const auto p = [colours] (const Subset& cubes) -> bool
		{
			return is_subset_possible(cubes, colours);
		};
		return std::all_of(game.begin(), game.end(), p);
	}

	std::size_t part_one(const std::vector<Game>& games)
	{
		Subset colours;
		colours.red = 12;
		colours.green = 13;
		colours.blue = 14;

		std::size_t sum = 0;
		for (std::size_t i = 0; i < games.size(); ++i)
		{
			const Game& game = games[i];
			if (is_game_possible(game, colours))
			{
				sum += i + 1;
			}
		}

		return sum;
	}

	int power(const Subset& cubes)
	{
		return cubes.red * cubes.green * cubes.blue;
	}

	/*
	 * The power of a set of cubes is equal to the number of red, green,
	 * and blue cubes multiplied together.
	 */
	int power(const Game& game)
	{
		Subset colours_required{};
		for (const Subset& cubes : game)
		{
			colours_required.red = std::max(colours_required.red, cubes.red);
			colours_required.green = std::max(colours_required.green, cubes.green);
			colours_required.blue = std::max(colours_required.blue, cubes.blue);
		}
		return power(colours_required);
	}

	/*
	 * Get the sum of the power of the sets
	 */
	int part_two(const std::vector<Game>& games)
	{
		int sum = 0;
		for (const Game& game : games)
		{
			sum += power(game);
		}
		return sum;
	}
}

TEST_CASE("Cube Conundrum", "[2]")
{
	const std::vector<Game> games = {
		{ {4, 0, 3}, {1, 2, 6}, {0, 2, 0} },
		{ {0, 2, 1}, {1, 3, 4}, {0, 1, 1} },
		{ {20, 8, 6}, {4, 13, 5}, {1, 5, 0} },
		{ {3, 1, 6}, {6, 3, 0}, {14, 3, 15} },
		{ {6, 3, 1}, {1, 2, 2 } }
	};
	REQUIRE(part_one(games) == 8);
	REQUIRE(power(games[0]) == 48);
	REQUIRE(part_two(games) == 2286);
}

SOLVE
{
	const std::vector<Game> games = parse_input(std::cin);

	std::cout << part_one(games) << std::endl;
	std::cout << part_two(games) << std::endl;
}
