#include "aoc.h"
#include "aoc/core.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

namespace
{
	void combine(std::string& recipes, S64& elf1, S64& elf2)
	{
		S64 sum = (recipes[elf1] - '0') + (recipes[elf2] - '0');
		const S64 index = std::ssize(recipes);
		do
		{
			recipes.push_back(static_cast<char>((sum % 10) + '0'));
			sum /= 10;
		} while (sum != 0);
		std::reverse(recipes.begin() + index, recipes.end());
		elf1 = (elf1 + (recipes[elf1] - '0') + 1) % std::ssize(recipes);
		elf2 = (elf2 + (recipes[elf2] - '0') + 1) % std::ssize(recipes);
	}

	std::string part_one(std::string recipes, const S64 num_recipes)
	{
		recipes.reserve(num_recipes + 12);
		S64 elf1 = 0;
		S64 elf2 = 1;

		while (std::ssize(recipes) < num_recipes + 10)
		{
			combine(recipes, elf1, elf2);
		}

		return recipes.substr(num_recipes, 10);
	}

	std::pair<S64, S64> try_match(const std::string& recipes, const std::string& needle, std::pair<S64, S64> indices)
	{
		if (indices.first == indices.second)
		{
			const auto first = std::find(recipes.begin() + indices.first, recipes.end(), needle[0]);
			if (first == recipes.end())
			{
				return { std::ssize(recipes), std::ssize(recipes) };
			}
			indices.first = first - recipes.begin();
			indices.second = indices.first + 1;
		}

		while (indices.second < std::ssize(recipes) && indices.second - indices.first < std::ssize(needle))
		{
			if (recipes[indices.second] != needle[indices.second - indices.first])
			{
				const auto first = std::find(recipes.begin() + indices.first + 1, recipes.end(), needle[0]);
				indices.first = first - recipes.begin();
				if (first == recipes.end())
				{
					indices.second = std::ssize(recipes);
				}
				else
				{
					indices.second = indices.first + 1;
				}
				return indices;
			}
			++indices.second;
		}

		return indices;
	}

	S64 part_two(std::string recipes, const std::string& needle)
	{
		S64 elf1 = 0;
		S64 elf2 = 1;
		std::pair<S64, S64> indices = { 0, 0 };
		while (true)
		{
			indices = try_match(recipes, needle, indices);
			if (indices.second - indices.first == std::ssize(needle))
			{
				return indices.first;
			}
			combine(recipes, elf1, elf2);
		}
	}
}

SOLVE
{
	const std::string recipes = "37";
	std::cout << part_one(recipes, 9) << '\n';
	std::cout << part_one(recipes, 5) << '\n';
	std::cout << part_one(recipes, 18) << '\n';
	std::cout << part_one(recipes, 2018) << '\n';
	std::cout << part_one(recipes, 330121) << '\n';

	std::cout << part_two(recipes, "51589") << '\n';
	std::cout << part_two(recipes, "01245") << '\n';
	std::cout << part_two(recipes, "92510") << '\n';
	std::cout << part_two(recipes, "59414") << '\n';
	std::cout << part_two(recipes, "330121") << '\n';
}