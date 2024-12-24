#include "aoc.h"

#include <format>
#include <iostream>
#include <ranges>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "aoc/core.h"

namespace
{
	std::unordered_map<std::string, U64> read_initial_values(std::istream& input)
	{
		std::unordered_map<std::string, U64> result;
		for (std::string line; std::getline(input, line);)
		{
			if (line.empty()) break;
			const std::regex re(R"((\w+): ([01]))");
			std::smatch match;
			if (!std::regex_match(line, match, re))
			{
				throw std::invalid_argument("Invalid input");
			}
			result[match[1]] = match[2] == "1" ? 1 : 0;
		}
		return result;
	}

	enum class Gate : U8
	{
		AND,
		OR,
		XOR,
	};

	struct Connection
	{
		std::string lhs;
		std::string rhs;
		Gate gate;
	};

	std::unordered_map<std::string, Connection> read_connections(std::istream& input)
	{
		std::unordered_map<std::string, Connection> result;
		for (std::string line; std::getline(input, line);)
		{
			const std::regex re(R"((\w+) (AND|OR|XOR) (\w+) -> (\w+))");
			std::smatch match;
			if (!std::regex_match(line, match, re))
			{
				throw std::invalid_argument("Invalid input");
			}
			result[match[4]] = { match[1], match[3], match[2] == "AND" ? Gate::AND : match[2] == "OR" ? Gate::OR : Gate::XOR };
		}
		return result;
	}

	U64 get_value(const std::string& key, std::unordered_map<std::string, U64>& values, const std::unordered_map<std::string, Connection>& connections)
	{
		if (const auto it = values.find(key); it != values.end())
		{
			return it->second;
		}

		const Connection& connection = connections.at(key);
		U64& value = values[key];
		switch (connection.gate)
		{
		case Gate::AND:
		{
			value = get_value(connection.lhs, values, connections) & get_value(connection.rhs, values, connections);
			break;
		}
		case Gate::OR:
		{
			value = get_value(connection.lhs, values, connections) | get_value(connection.rhs, values, connections);
			break;
		}
		case Gate::XOR:
		{
			value = get_value(connection.lhs, values, connections) ^ get_value(connection.rhs, values, connections);
			break;
		}
		}
		return value;
	}

	U64 get_number(const char letter, std::unordered_map<std::string, U64> values, const std::unordered_map<std::string, Connection>& connections)
	{
		U64 result = 0;
		for (const std::string& key : std::ranges::views::keys(connections))
		{
			const std::regex re(std::format("{}(\\d+)", letter));
			std::smatch match;
			if (std::regex_match(key, match, re))
			{
				const U64 index = std::stoull(match[1]);
				result |= get_value(key, values, connections) << index;
			}
		}
		return result;
	}

	U64 part_one(const std::unordered_map<std::string, U64>& values, const std::unordered_map<std::string, Connection>& connections)
	{
		return get_number('z', values, connections);
	}

	std::string join(const std::vector<std::string>& strings, const char delimiter)
	{
		if (strings.empty()) return "";
		std::string result;
		result.reserve(strings.size() * 3 + (strings.size() - 1));
		result.append(strings.front());
		for (S64 i = 1; i < std::ssize(strings); ++i)
		{
			result.append(1, delimiter);
			result.append(strings[i]);
		}
		return result;
	}

	std::string part_two(const std::unordered_map<std::string, U64>& values, const std::unordered_map<std::string, Connection>& connections)
	{
		const S64 num_bits = std::ssize(values) / 2;
		std::vector<std::string> bad_outputs;

		for (S64 i = 1; i < num_bits; ++i)
		{
			const std::string key = std::format("z{:02}", i);
			const Connection& connection = connections.at(key);
			if (connection.gate != Gate::XOR || (connection.lhs[0] == 'x' || connection.rhs[0] == 'x'))
			{
				bad_outputs.push_back(key);
			}
		}

		for (const auto& pair : connections)
		{
			if (pair.second.gate == Gate::XOR)
			{
				if (pair.second.lhs[0] == 'x' || pair.second.rhs[0] == 'x')
				{
					// pair.first should now be used in an AND and an XOR gate
					for (const auto& other : connections)
					{
						if (other.second.lhs == pair.first || other.second.rhs == pair.first)
						{
							if (other.second.gate == Gate::OR)
							{
								bad_outputs.push_back(pair.first);
							}
						}
					}
				}
				else if (pair.first[0] != 'z')
				{
					bad_outputs.push_back(pair.first);
				}
			}
			else if (pair.second.gate == Gate::AND)
			{
				if (pair.second.lhs[0] == 'x' || pair.second.rhs[0] == 'x')
				{
					if (pair.second.lhs == "x00" || pair.second.rhs == "x00")
					{
						continue;
					}
					// pair.first should now be used in an OR gate
					for (const auto& other : connections)
					{
						if (other.second.lhs == pair.first || other.second.rhs == pair.first)
						{
							if (other.second.gate != Gate::OR)
							{
								bad_outputs.push_back(pair.first);
								break;
							}
						}
					}
				}
			}
		}

		std::ranges::sort(bad_outputs);
		return join(bad_outputs, ',');
	}
}

SOLVE
{
	const std::unordered_map<std::string, U64> initial_values = read_initial_values(std::cin);
	const std::unordered_map<std::string, Connection> connections = read_connections(std::cin);

	std::cout << part_one(initial_values, connections) << '\n';
	std::cout << part_two(initial_values, connections) << '\n';
}