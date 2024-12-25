#include "aoc.h"
#include "aoc/core.h"

#include <iostream>
#include <string>
#include <vector>

namespace
{
	std::vector<std::vector<std::string>> read_input(std::istream& input)
	{
		std::vector<std::vector<std::string>> result;
		std::vector<std::string> group;
		for (std::string line; std::getline(input, line);)
		{
			if (line.empty())
			{
				result.push_back(std::move(group));
				group.clear();
			}
			else
			{
				group.push_back(line);
			}
		}
		if (!group.empty())
		{
			result.push_back(std::move(group));
		}
		return result;
	}

	std::vector<S64> convert_lock_to_heights(const std::vector<std::string>& lock)
	{
		std::vector<S64> heights;
		for (S64 x = 0; x < std::ssize(lock[0]); ++x)
		{
			S64 y = 1;
			while (y < std::ssize(lock) - 1 && lock[y][x] == '#')
			{
				++y;
			}
			heights.push_back(y - 1);
		}
		return heights;
	}

	std::vector<S64> convert_key_to_heights(const std::vector<std::string>& key)
	{
		const S64 max_height = std::ssize(key) - 2;
		std::vector<S64> heights;
		for (S64 x = 0; x < std::ssize(key[0]); ++x)
		{
			S64 y = max_height;
			while (y > 0 && key[y][x] == '#')
			{
				--y;
			}
			heights.push_back(max_height - y);
		}
		return heights;
	}

	bool fits(const std::vector<S64>& lock, const std::vector<S64>& key)
	{
		constexpr S64 max_height = 5;
		for (S64 i = 0; i < std::ssize(lock); ++i)
		{
			if (lock[i] + key[i] > max_height)
			{
				return false;
			}
		}
		return true;
	}

	S64 part_one(const std::vector<std::vector<std::string>>& schematics)
	{
		std::vector<std::vector<S64>> locks;
		std::vector<std::vector<S64>> keys;

		for (const std::vector<std::string>& schematic : schematics)
		{
			if (schematic[0][0] == '#')
			{
				locks.push_back(convert_lock_to_heights(schematic));
			}
			else
			{
				keys.push_back(convert_key_to_heights(schematic));
			}
		}

		S64 result = 0;
		for (S64 i = 0; i < std::ssize(locks); ++i)
		{
			for (S64 j = 0; j < std::ssize(keys); ++j)
			{
				if (fits(locks[i], keys[j]))
				{
					++result;
				}
			}
		}
		return result;
	}
}

SOLVE
{
	const auto schematics = read_input(std::cin);
	std::cout << part_one(schematics) << '\n';
}