#include "aoc.h"
#include "aoc/core.h"
#include "aoc/orbit_structure.h"

#include <bitset>
#include <iostream>
#include <numeric>
#include <regex>
#include <set>
#include <string>

namespace
{
	struct State
	{
		S64 offset;
		std::set<S64> values;
	};

	bool operator==(const State& lhs, const State& rhs)
	{
		return lhs.values == rhs.values;
	}

	State read_initial_state(std::istream& input)
	{
		std::string line;
		std::getline(input, line);
		std::set<S64> initial_state;
		for (S64 i = 15; i < std::ssize(line); ++i)
		{
			if (line[i] == '#')
			{
				initial_state.insert(i - 15);
			}
		}
		return { .offset = 0, .values = std::move(initial_state) };
	}

	std::pair<S64, S64> parse_rule(const std::string& line)
	{
		static const std::regex re(R"(([.#]{5}) => ([.#]))");
		std::smatch match;
		std::regex_match(line, match, re);
		S64 key = 0;
		for (int i = 0; i < 5; ++i)
		{
			if (line[i] == '#')
			{
				key |= 1ll << i;
			}
		}
		return { key, match[2].str()[0] == '#' ? 1 : 0 };
	}

	std::bitset<32> read_rules(std::istream& input)
	{
		std::bitset<32> rules;
		for (std::string line; std::getline(input, line);)
		{
			if (line.empty()) continue;
			const std::pair<S64, S64> rule = parse_rule(line);
			rules.set(rule.first, rule.second);
		}
		return rules;
	}

	S64 get_index(const std::set<S64>& state, const S64 value)
	{
		S64 index = 0;
		for (S64 i = value - 2; i <= value + 2; ++i)
		{
			if (state.contains(i))
			{
				index |= 1ll << (i - value + 2);
			}
		}
		return index;
	}

	State step(const State& state, const std::bitset<32>& rules)
	{
		std::set<S64> next_state;
		const S64 max = *state.values.rbegin() + 2;
		S64 offset = std::numeric_limits<S64>::max();
		for (S64 i = -2; i <= max; ++i)
		{
			const S64 index = get_index(state.values, i);
			if (rules.test(index))
			{
				if (offset == std::numeric_limits<S64>::max())
				{
					offset = i;
				}
				next_state.insert(i - offset);
			}
		}
		return { .offset = offset + state.offset, .values = std::move(next_state) };
	}

	struct Stepper
	{
		std::bitset<32> rules;
		State operator()(const State& state) const
		{
			return step(state, rules);
		}
	};

	S64 sum_plants(const State& state)
	{
		const S64 total = state.offset * std::ssize(state.values);
		return std::accumulate(state.values.begin(), state.values.end(), total);
	}
	
	S64 part_one(const State& state, const std::bitset<32>& rules)
	{
		return sum_plants(aoc::power(state, Stepper{ rules }, 20));
	}

	S64 part_two(State state, const std::bitset<32>& rules)
	{
		const Stepper stepper{ rules };
		const aoc::Orbit_structure<State> orbit = aoc::orbit_structure_nonterminating_orbit(state, stepper);
		state = orbit.connection_point;
		// @note The offset increases by 1 each generation after the connection point.
		state.offset += static_cast<S64>(50000000000 - orbit.handle_size);
		return sum_plants(state);
	}
}

SOLVE
{
	const State initial_state = read_initial_state(std::cin);
	const std::bitset<32> rules = read_rules(std::cin);

	std::cout << part_one(initial_state, rules) << '\n';
	std::cout << part_two(initial_state, rules) << '\n';
}