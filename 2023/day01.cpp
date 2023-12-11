#include "aoc.h"
#include <aoc/algorithm.h>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace
{
	std::vector<std::string> read_lines(std::istream& is)
	{
		using I = std::istream_iterator<std::string>;
		return { I{is}, I{} };
	}

	int get_calibration_value_digits(const std::string& line)
	{
		using Iter = typename std::string::const_iterator;
		const Iter first_digit = std::find_if(line.begin(), line.end(), ::isdigit);
		const Iter last_digit = aoc::find_if_backward(first_digit, line.end(), ::isdigit);
		const int calibration_value = (*first_digit - '0') * 10 + (*std::prev(last_digit) - '0');
		return calibration_value;
	}

	int part_one(const std::vector<std::string>& lines)
	{
		int sum = 0;
		for (const std::string& line : lines)
		{
			sum += get_calibration_value_digits(line);
		}
		return sum;
	}

	int get_calibration_value(const std::string& line)
	{
		static const std::unordered_map<std::string_view, int> numbers = {
			{ "1", 1 },
			{ "2", 2 },
			{ "3", 3 },
			{ "4", 4 },
			{ "5", 5 },
			{ "6", 6 },
			{ "7", 7 },
			{ "8", 8 },
			{ "9", 9 },
			{ "one", 1 },
			{ "two", 2 },
			{ "three", 3 },
			{ "four", 4 },
			{ "five", 5 },
			{ "six", 6 },
			{ "seven", 7 },
			{ "eight", 8 },
			{ "nine", 9 },
		};
		using Pair = std::pair<std::string::size_type, int>;
		std::vector<Pair> values;
		for (const std::pair<const std::string_view, int>& mapping : numbers)
		{
			std::string::size_type pos = 0;
			while (true)
			{
				pos = line.find(mapping.first, pos);
				if (pos == std::string::npos)
				{
					break;
				}
				values.emplace_back(pos, mapping.second);
				pos += mapping.first.size();
			}
		}

		std::sort(values.begin(), values.end(), [] (const Pair& x, const Pair& y) -> bool
			  {
				  return x.first < y.first;
			  });
		return values.front().second * 10 + values.back().second;
	}

	int part_two(const std::vector<std::string>& lines)
	{
		int sum = 0;
		for (const std::string& line : lines)
		{
			sum += get_calibration_value(line);
		}
		return sum;
	}
}

SOLVE
{
	const std::vector<std::string> lines = read_lines(std::cin);

	std::cout << part_one(lines) << std::endl;
	std::cout << part_two(lines) << std::endl;
}
