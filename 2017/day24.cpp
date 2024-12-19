#include "aoc.h"
#include "aoc/core.h"

#include <iostream>
#include <regex>
#include <set>
#include <string>
#include <vector>

namespace
{
	using Pair = std::pair<S64, S64>;

	Pair read_pair(const std::string& line)
	{
		static const std::regex regex(R"((\d+)/(\d+))");
		std::smatch match;
		std::regex_match(line, match, regex);
		return { std::stoll(match[1]), std::stoll(match[2]) };
	}

	std::vector<Pair> read_input(std::istream& in)
	{
		std::vector<Pair> pairs;
		for (std::string line; std::getline(in, line);)
		{
			pairs.push_back(read_pair(line));
		}
		return pairs;
	}

	S64 find_best_score(const std::vector<Pair>& pairs, std::set<const Pair*>& visited, const S64 pin, const S64 current, S64 best)
	{
		best = std::max(best, current);
		for (const Pair& pair : pairs)
		{
			if (visited.contains(&pair)) continue;
			if (pair.first != pin && pair.second != pin) continue;
			const S64 new_total = current + pair.first + pair.second;
			const S64 new_pin = pair.first == pin ? pair.second : pair.first;
			visited.insert(&pair);
			best = find_best_score(pairs, visited, new_pin, new_total, best);
			visited.erase(&pair);
		}
		return best;
	}

	Pair find_longest_score(const std::vector<Pair>& pairs, std::set<const Pair*>& visited, const S64 pin, const S64 total, const S64 length, Pair best)
	{
		if (length > best.second)
		{
			best = { total, length };
		}
		else if (length == best.second)
		{
			best.first = std::max(best.first, total);
		}

		for (const Pair& pair : pairs)
		{
			if (visited.contains(&pair)) continue;
			if (pair.first != pin && pair.second != pin) continue;

			visited.insert(&pair);
			const S64 new_total = total + pair.first + pair.second;
			const S64 new_pin = pair.first == pin ? pair.second : pair.first;
			best = find_longest_score(pairs, visited, new_pin, new_total, length + 1, best);
			visited.erase(&pair);
		}
		return best;
	}

	S64 part_one(const std::vector<Pair>& pairs)
	{
		std::set<const Pair*> visited;
		return find_best_score(pairs, visited, 0, 0, 0);
	}

	S64 part_two(const std::vector<Pair>& pairs)
	{
		std::set<const Pair*> visited;
		return find_longest_score(pairs, visited, 0, 0, 0, { 0, 0 }).first;
	}
}

SOLVE
{
	const std::vector<Pair> pairs = read_input(std::cin);

	std::cout << part_one(pairs) << '\n';
	std::cout << part_two(pairs) << '\n';
}