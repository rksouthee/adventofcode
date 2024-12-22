#include "aoc.h"
#include "aoc/core.h"
#include "aoc/orbit_structure.h"

#include <array>
#include <iostream>
#include <numeric>
#include <span>
#include <string>
#include <unordered_map>
#include <vector>
#include <catch2/catch_test_macros.hpp>

namespace
{
	U64 mix(const U64 a, const U64 b)
	{
		return a ^ b;
	}

	U64 prune(const U64 a)
	{
		return a % 16777216;
	}

	U64 generate(U64 secret)
	{
		secret = prune(mix(secret * 64, secret));
		secret = prune(mix(secret / 32, secret));
		secret = prune(mix(secret * 2048, secret));
		return secret;
	}

	std::vector<U64> read_input(std::istream& in)
	{
		using I = std::istream_iterator<U64>;
		return { I{in}, I{} };
	}

	U64 part_one(const std::vector<U64>& initial_secret_numbers)
	{
		U64 result = 0;
		for (const U64 secret : initial_secret_numbers)
		{
			const U64 new_secret = aoc::power(secret, generate, 2000);
			result += new_secret;
		}
		return result;
	}


	using Key = std::tuple<S64, S64, S64, S64>;
	using Map = std::unordered_map<U64, S64>;

	U64 from_key(const Key& key)
	{
		const U64 a = static_cast<U64>(std::get<0>(key) + 9);
		const U64 b = static_cast<U64>(std::get<1>(key) + 9);
		const U64 c = static_cast<U64>(std::get<2>(key) + 9);
		const U64 d = static_cast<U64>(std::get<3>(key) + 9);
		return a | (b << 5) | (c << 10) | (d << 15);
	}

	Map get_best_prices(U64 secret)
	{
		Map map;
		S64 last_price = 0;
		std::vector<S64> differences;
		differences.reserve(1999);
		for (S64 i = 0; i < 2000; ++i)
		{
			const auto price = static_cast<S64>(secret) % 10;
			differences.push_back(price - last_price);
			if (differences.size() >= 4)
			{
				const std::tuple key = { differences[differences.size() - 4], differences[differences.size() - 3], differences[differences.size() - 2], differences[differences.size() - 1] };
				map.try_emplace(from_key(key), price);
			}
			secret = generate(secret);
			last_price = price;
		}
		return map;
	}

	U64 part_two(const std::vector<U64>& initial_secret_numbers)
	{
		Map best_prices;
		S64 max = 0;
		for (const U64 secret : initial_secret_numbers)
		{
			const auto map = get_best_prices(secret);
			for (const auto& [key, value] : map)
			{
				best_prices[key] += value;
				max = std::max(max, best_prices[key]);
			}
		}
		return max;
	}
}

TEST_CASE("2020 22 01", "[2020][22][01]")
{
	REQUIRE(mix(42, 15) == 37);
	REQUIRE(prune(100000000) == 16113920);
	REQUIRE(generate(15887950) == 16495136);
}

SOLVE
{
	const std::vector<U64> initial_secret_numbers = read_input(std::cin);

	std::cout << part_one(initial_secret_numbers) << '\n';
	std::cout << part_two(initial_secret_numbers) << '\n';
}