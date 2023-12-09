#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

#include <cstdint>

namespace
{
	struct Input
	{
		std::string time;
		std::string distance;
	};

	/*
	 * Read a line of the form "string:    123  34   345" and return the
	 * substring starting from the first digit, so in our example that
	 * would return "123  34   345"
	 */
	std::string read_line(std::istream& is)
	{
		std::string line;
		std::getline(is, line);
		const auto iter = std::find_if(line.begin(), line.end(), ::isdigit);
		return line.substr(iter - line.begin());
	}

	Input read_input(std::istream& is)
	{
		Input input;
		input.time = read_line(is);
		input.distance = read_line(is);
		return input;
	}

	std::vector<std::uint64_t> read_values(const std::string& str)
	{
		using I = std::istream_iterator<std::uint64_t>;
		std::istringstream iss(str);
		return { I{iss}, I{} };
	}

	/**
	 * We want to find the integer solution that satisfies
	 *	n^2 - tn + d = 0
	 */
	std::uint64_t solve_quadratic(std::uint64_t t, std::uint64_t d)
	{
		double discriminant = (double)(t * t) - 4.0 * (double)d;
		return std::ceil((t - std::sqrt(discriminant)) / 2.0);
	}

	std::uint64_t get_number_of_winners(const std::uint64_t time, const std::uint64_t distance)
	{
		const std::uint64_t n = solve_quadratic(time, distance + 1);
		return time - 2 * n + 1;
	}

	std::uint64_t part_one(const Input& input)
	{
		const std::vector<std::uint64_t> times = read_values(input.time);
		const std::vector<std::uint64_t> distances = read_values(input.distance);
		std::uint64_t product = 1;
		for (std::size_t i = 0; i < times.size(); ++i)
		{
			const std::uint64_t time = times[i];
			const std::uint64_t distance = distances[i];
			product *= get_number_of_winners(time, distance);
		}
		return product;
	}

	std::uint64_t read_value(std::string str)
	{
		str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
		return std::stoull(str);
	}

	std::uint64_t part_two(const Input& input)
	{
		return get_number_of_winners(read_value(input.time), read_value(input.distance));
	}
}

extern "C" void solve(std::istream& is)
{
	const Input input = read_input(is);
	std::cout << part_one(input) << std::endl;
	std::cout << part_two(input) << std::endl;
}
