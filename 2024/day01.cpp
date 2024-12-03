#include "aoc.h"

#include <cassert>

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace
{
	using List = std::vector<int>;
	std::pair<List, List> read_input(std::istream& in)
	{
		List list0, list1;
		std::string line;
		while (std::getline(in, line))
		{
			std::istringstream iss(line);
			int a, b;
			iss >> a >> b;
			list0.push_back(a);
			list1.push_back(b);
		}
		return { list0, list1 };
	}

	int part_one(List list0, List list1)
	{
		assert(list0.size() == list1.size());
		int result = 0;
		std::sort(list0.begin(), list0.end());
		std::sort(list1.begin(), list1.end());
		for (std::size_t i = 0; i < list0.size(); ++i)
		{
			result += std::abs(list0[i] - list1[i]);
		}
		return result;
	}

	std::unordered_map<int, int> make_histogram(const List& list)
	{
		std::unordered_map<int, int> histogram;
		for (const int val : list)
		{
			histogram[val]++;
		}
		return histogram;
	}

	int part_two(const List& list0, const List& list1)
	{
		const auto histogram0 = make_histogram(list0);
		const auto histogram1 = make_histogram(list1);
		int result = 0;
		for (const auto& [key, value] : histogram0)
		{
			const auto iter = histogram1.find(key);
			if (iter == histogram1.end())
			{
				continue;
			}

			result += value * key * iter->second;
		}
		return result;
	}
}

SOLVE
{
	const auto [list0, list1] = read_input(std::cin);

	std::cout << part_one(list0, list1) << std::endl;
	std::cout << part_two(list0, list1) << std::endl;
}
