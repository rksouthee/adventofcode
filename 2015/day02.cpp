#include "aoc.h"
#include "aoc/core.h"

#include <iostream>
#include <regex>
#include <string>

namespace
{
	struct Dimensions
	{
		S64 length;
		S64 width;
		S64 height;
	};

	Dimensions parse(const std::string& str)
	{
		const std::regex regex(R"((\d+)x(\d+)x(\d+))");
		std::smatch match;
		std::regex_match(str, match, regex);
		return { std::stoll(match[1]), std::stoll(match[2]), std::stoll(match[3]) };
	}

	std::vector<Dimensions> read_input(std::istream& input)
	{
		std::vector<Dimensions> result;
		std::string line;
		while (std::getline(input, line))
		{
			result.push_back(parse(line));
		}
		return result;
	}

	S64 surface_area(const Dimensions& dimensions)
	{
		return 2 * (dimensions.length * dimensions.width + dimensions.width * dimensions.height + dimensions.height * dimensions.length);
	}

	S64 area_smallest_side(const Dimensions& dimensions)
	{
		return std::min({ dimensions.length * dimensions.width, dimensions.width * dimensions.height, dimensions.height * dimensions.length });
	}

	S64 part_one(const std::vector<Dimensions>& input)
	{
		S64 result = 0;
		for (const Dimensions& dimensions : input)
		{
			result += surface_area(dimensions) + area_smallest_side(dimensions);
		}
		return result;
	}

	S64 smallest_perimeter(const Dimensions& dimensions)
	{
		return 2 * (dimensions.length + dimensions.width + dimensions.height - std::max({ dimensions.length, dimensions.width, dimensions.height }));
	}

	S64 volume(const Dimensions& dimensions)
	{
		return dimensions.length * dimensions.width * dimensions.height;
	}

	S64 part_two(const std::vector<Dimensions>& dimensions)
	{
		S64 result = 0;
		for (const Dimensions& dimension : dimensions)
		{
			result += smallest_perimeter(dimension) + volume(dimension);
		}
		return result;
	}
}

SOLVE
{
	const std::vector<Dimensions> input = read_input(std::cin);

	std::cout << part_one(input) << '\n';
	std::cout << part_two(input) << '\n';
}