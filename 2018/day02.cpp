#include "aoc.h"
#include <aoc/algorithm.h>

#include <algorithm>
#include <array>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

namespace
{
	std::vector<std::string> read_input(std::istream& is)
	{
		using I = std::istream_iterator<std::string>;
		return { I{is}, I{} };
	}

	std::array<int, 26> count_chars(const std::string& box_id)
	{
		std::array<int, 26> counts{};
		for (const char ch : box_id)
		{
			++counts[ch - 'a'];
		}
		return counts;
	}

	bool has_pair(const std::array<int, 26>& counts)
	{
		return std::find(counts.begin(), counts.end(), 2) != counts.end();
	}

	bool has_triple(const std::array<int, 26>& counts)
	{
		return std::find(counts.begin(), counts.end(), 3) != counts.end();
	}

	int part_one(const std::vector<std::string>& box_ids)
	{
		int pairs = 0;
		int triples = 0;
		for (const std::string& box_id : box_ids)
		{
			const std::array<int, 26> counts = count_chars(box_id);
			if (has_pair(counts))
			{
				++pairs;
			}
			if (has_triple(counts))
			{
				++triples;
			}
		}
		return pairs * triples;
	}

	typename std::string::const_iterator string_diff(const std::string& first, const std::string& second)
	{
		using Iter = typename std::string::const_iterator;
		using Pair = std::pair<Iter, Iter>;
		Pair pair = aoc::find_mismatch(first.begin(), first.end(), second.begin(), second.end());
		if (pair.first != first.end())
		{
			Pair temp = aoc::find_mismatch(std::next(pair.first), first.end(), std::next(pair.second), second.end());
			if (temp.first == first.end())
			{
				return pair.first;
			}
		}
		return first.end();
	}

	std::string part_two(const std::vector<std::string>& box_ids)
	{
		for (std::size_t i = 0; i < box_ids.size(); ++i)
		{
			for (std::size_t j = i + 1; j < box_ids.size(); ++j)
			{
				const std::string& first = box_ids[i];
				const std::string& second = box_ids[j];
				const typename std::string::const_iterator iter = string_diff(first, second);
				if (iter != first.end())
				{
					std::string result(first.begin(), iter);
					result.insert(result.end(), std::next(iter), first.end());
					return result;
				}
			}
		}
		return {};
	}
}

SOLVE
{
	const std::vector<std::string> box_ids = read_input(std::cin);

	std::cout << part_one(box_ids) << std::endl;
	std::cout << part_two(box_ids) << std::endl;
}
