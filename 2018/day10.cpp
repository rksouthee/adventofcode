#include "aoc.h"
#include "aoc/vector.h"

#include <iostream>
#include <string>
#include <vector>
#include <regex>

using aoc::Vector2;

namespace
{
	struct Light
	{
		Vector2 position;
		Vector2 velocity;
	};

	Light parse_light(const std::string& line)
	{
		static const std::regex re(R"(position=<\s*(-?\d+),\s*(-?\d+)> velocity=<\s*(-?\d+),\s*(-?\d+)>)");
		std::smatch match;
		std::regex_match(line, match, re);
		return Light{ { std::stoll(match[1]), std::stoll(match[2]) }, { std::stoll(match[3]), std::stoll(match[4]) } };
	}

	std::vector<Light> read_input(std::istream& in)
	{
		std::vector<Light> lights;
		for (std::string line; std::getline(in, line);)
		{
			lights.push_back(parse_light(line));
		}
		return lights;
	}

	void step(std::vector<Light>& lights)
	{
		for (Light& light : lights)
		{
			light.position += light.velocity;
		}
	}

	std::vector<std::string> get_message(const std::vector<Light>& lights)
	{
		S64 min_x, min_y, max_x, max_y;
		min_x = min_y = std::numeric_limits<S64>::max();
		max_x = max_y = std::numeric_limits<S64>::min();

		for (const Light& light : lights)
		{
			min_x = std::min(min_x, light.position.x);
			min_y = std::min(min_y, light.position.y);
			max_x = std::max(max_x, light.position.x);
			max_y = std::max(max_y, light.position.y);
		}

		const S64 width = max_x - min_x + 1;
		const S64 height = max_y - min_y + 1;
		if (width > 128 || height > 12) return {};
		std::vector grid(height, std::string(width, '.'));
		for (const Light& light : lights)
		{
			const S64 x = light.position.x - min_x;
			const S64 y = light.position.y - min_y;
			grid[y][x] = '#';
		}
		return grid;
	}

	void print_grid(const std::vector<std::string>& grid)
	{
		for (const std::string& line : grid)
		{
			std::cout << line << '\n';
		}
	}
}

SOLVE
{
	std::vector<Light> lights = read_input(std::cin);
	S64 seconds = 0;
	while (true)
	{
		++seconds;
		step(lights);
		const std::vector<std::string> message = get_message(lights);
		if (message.empty()) continue;
		std::cout << "After " << seconds << " seconds:\n";
		print_grid(message);
		break;
	}
}