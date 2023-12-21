#include "aoc.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include <cassert>
#include <cstdint>

namespace
{
	using S64 = std::int64_t;

	struct Instruction
	{
		char direction;
		S64 meters;
		std::string colour;
	};

	std::istream& operator>>(std::istream& is, Instruction& x)
	{
		is >> x.direction;
		if (x.direction != 'R' && x.direction != 'D' && x.direction != 'L' && x.direction != 'U')
		{
			is.setstate(std::ios_base::eofbit);
			return is;
		}
		return is >> x.meters >> x.colour;
	}

	std::vector<Instruction> read_input(std::istream& is)
	{
		using I = std::istream_iterator<Instruction>;
		return { I{is}, I{} };
	}

	struct Point
	{
		S64 x;
		S64 y;
	};

	std::vector<Point> get_points_1(const std::vector<Instruction>& plan)
	{
		Point curr{0, 0};
		std::vector<Point> points = { curr };
		for (const Instruction& instruction : plan)
		{
			switch (instruction.direction)
			{
			case 'R': curr.x += instruction.meters; break;
			case 'D': curr.y += instruction.meters; break;
			case 'L': curr.x -= instruction.meters; break;
			case 'U': curr.y -= instruction.meters; break;
			}
			points.push_back(curr);
		}
		return points;
	}

	std::vector<Point> get_points_2(const std::vector<Instruction>& plan)
	{
		Point curr{0, 0};
		std::vector<Point> points = { curr };
		for (const Instruction& instruction : plan)
		{
			const std::string& colour = instruction.colour;
			assert(colour.size() == 9);
			const S64 meters = std::stoll(colour.substr(2, 5), nullptr, 16);
			switch (colour[7])
			{
			case '0': curr.x += meters; break;
			case '1': curr.y += meters; break;
			case '2': curr.x -= meters; break;
			case '3': curr.y -= meters; break;
			}
			points.push_back(curr);
		}
		return points;
	}

	S64 get_perimeter(const std::vector<Point>& points)
	{
		S64 perimeter = 0;
		for (std::size_t i = 1; i < points.size(); ++i)
		{
			perimeter += std::abs(points[i - 1].x - points[i].x) + std::abs(points[i - 1].y - points[i].y);
		}
		return perimeter;
	}

	S64 get_internal_area(const std::vector<Point>& points)
	{
		S64 internal_area = 0;
		for (std::size_t i = 1; i < points.size(); ++i)
		{
			internal_area += (points[i - 1].x - points[i].x) * (points[i - 1].y + points[i].y);
		}
		return internal_area / 2;
	}

	S64 get_area(const std::vector<Point>& points)
	{
		S64 perimeter = get_perimeter(points);
		S64 internal_area = get_internal_area(points);
		S64 area = internal_area + (perimeter / 2) + 1;
		return area;
	}

	S64 part_one(const std::vector<Instruction>& plan)
	{
		const std::vector<Point> points = get_points_1(plan);
		return get_area(points);
	}

	S64 part_two(const std::vector<Instruction>& plan)
	{
		const std::vector<Point> points = get_points_2(plan);
		return get_area(points);
	}
}

SOLVE
{
	const std::vector<Instruction> plan = read_input(std::cin);

	std::cout << part_one(plan) << std::endl;
	std::cout << part_two(plan) << std::endl;
}
