#include <iostream>
#include <numeric>
#include <regex>
#include <string>
#include <unordered_map>

namespace
{
	struct Input
	{
		std::string directions;
		std::unordered_map<std::string, std::pair<std::string, std::string>> nodes;
	};

	Input read_input(std::istream& is)
	{
		Input input;
		std::getline(is, input.directions);
		std::string line;
		std::getline(is, line);
		const std::regex re("([A-Z]{3}) = \\(([A-Z]{3}), ([A-Z]{3})\\)");
		while (std::getline(is, line))
		{
			std::smatch match;
			if (std::regex_match(line, match, re))
			{
				input.nodes[match[1].str()] = std::make_pair(match[2].str(), match[3].str());
			}
		}
		return input;
	}

	std::string step(const Input& input, const std::string& node, std::size_t n)
	{
		const std::pair<std::string, std::string>& options = input.nodes.at(node);
		const char direction = input.directions[n % input.directions.size()];
		if (direction == 'L')
		{
			return options.first;
		}
		return options.second;
	}

	std::size_t get_steps(const Input& input, std::string node, bool (*p)(const std::string&))
	{
		std::size_t steps = 0;
		while (!p(node))
		{
			node = step(input, node, steps);
			++steps;
		}
		return steps;
	}

	bool is_zzz(const std::string& node) { return node == "ZZZ"; }

	std::size_t part_one(const Input& input)
	{
		return get_steps(input, "AAA", is_zzz);
	}

	bool ends_with_z(const std::string& node) { return node.back() == 'Z'; }

	std::size_t part_two(const Input& input)
	{
		std::size_t lcm = 1;
		for (const auto& p : input.nodes)
		{
			if (p.first.back() == 'A')
			{
				lcm = std::lcm(lcm, get_steps(input, p.first, ends_with_z));
			}
		}
		return lcm;
	}
}

extern "C" void solve(std::istream& is)
{
	const Input input = read_input(is);

	std::cout << part_one(input) << std::endl;
	std::cout << part_two(input) << std::endl;
}
