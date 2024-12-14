#include "aoc.h"
#include "aoc/vector.h"

#include <iostream>
#include <regex>
#include <set>
#include <string>
#include <vector>

using aoc::Vector2;

namespace
{
	struct Robot
	{
		Vector2 position;
		Vector2 velocity;
	};

	std::vector<Robot> read_input(std::istream& input)
	{
		std::vector<Robot> robots;
		std::string line;
		while (std::getline(input, line))
		{
			std::regex regex(R"(p=(-?\d+),(-?\d+) v=(-?\d+),(-?\d+))");
			std::smatch match;
			if (std::regex_match(line, match, regex))
			{
				Robot robot;
				robot.position.x = std::stoi(match[1]);
				robot.position.y = std::stoi(match[2]);
				robot.velocity.x = std::stoi(match[3]);
				robot.velocity.y = std::stoi(match[4]);
				robots.push_back(robot);
			}
		}
		return robots;
	}

	Vector2 iterate(const Robot& robot, const S64 seconds, const S64 width, const S64 height)
	{
		S64 y = (robot.position.y + robot.velocity.y * seconds) % height;
		if (y < 0)
		{
			y += height;
		}
		S64 x = (robot.position.x + robot.velocity.x * seconds) % width;
		if (x < 0)
		{
			x += width;
		}
		return { x, y };
	}

	S64 get_quadrant_index(const Vector2& position, const S64 width, const S64 height)
	{
		if (position.x < width / 2)
		{
			if (position.y < height / 2)
			{
				return 0;
			}

			if (position.y > height / 2)
			{
				return 1;
			}
		}
		else if (position.x > width / 2)
		{
			if (position.y < height / 2)
			{
				return 2;
			}

			if (position.y > height / 2)
			{
				return 3;
			}
		}
		return -1;
	}

	S64 part_one(const std::vector<Robot>& robots, const S64 width, const S64 height, const S64 seconds)
	{
		S64 counts[4] = { 0 };
		for (const Robot& robot : robots)
		{
			const Vector2 position = iterate(robot, seconds, width, height);
			const S64 index = get_quadrant_index(position, width, height);
			if (index != -1)
			{
				++counts[index];
			}
		}
		return counts[0] * counts[1] * counts[2] * counts[3];
	}

	void draw_robots(const std::vector<Robot>& robots, const S64 width, const S64 height)
	{
		std::vector grid(height, std::string(width, '.'));
		for (const Robot& robot : robots)
		{
			grid[robot.position.y][robot.position.x] = '#';
		}

		for (const std::string& line : grid)
		{
			std::cout << line << '\n';
		}
	}

	S64 part_two(std::vector<Robot> robots, const S64 width, const S64 height)
	{
		S64 seconds = 0;
		while (true)
		{
			++seconds;
			std::set<Vector2> positions;
			for (Robot& robot : robots)
			{
				robot.position = iterate(robot, 1, width, height);
				positions.insert(robot.position);
			}

			if (positions.size() == robots.size())
			{
				draw_robots(robots, width, height);
				return seconds;
			}
		}
	}
}

SOLVE
{
	const std::vector<Robot> robots = read_input(std::cin);

	// Example
	//std::cout << part_one(robots, 11, 7, 100) << '\n';

	constexpr S64 width = 101;
	constexpr S64 height = 103;
	constexpr S64 seconds = 100;
	std::cout << part_one(robots, width, height, seconds) << '\n';
	std::cout << part_two(robots, width, height) << '\n';
}