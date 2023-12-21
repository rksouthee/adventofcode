#include "aoc.h"
#include <aoc/algorithm.h>
#include <aoc/string_helpers.h>

#include <catch2/catch_test_macros.hpp>

#include <array>
#include <iostream>
#include <numeric>
#include <regex>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include <cstdint>

namespace
{
	using S64 = std::int64_t;
	using Part = std::array<S64, 4>;

	S64 parse_value(const std::string_view view)
	{
		S64 value = 0;
		aoc::parse_nonnegative_integer(view.begin() + 2, view.end(), value);
		return value;
	}

	Part parse_part(std::string_view view)
	{
		Part part{};
		view = view.substr(1, view.size() - 2);
		const std::vector<std::string_view> views = aoc::split(view, ',');
		assert(views.size() == 4);
		for (std::size_t i = 0; i < 4; ++i)
		{
			part[i] = parse_value(views[i]);
		}
		return part;
	}

	struct Rule
	{
		int key;
		char op;
		S64 value;
		std::string destination;
	};

	int key_from_char(char ch)
	{
		switch (ch)
		{
		case 'x': return 0;
		case 'm': return 1;
		case 'a': return 2;
		case 's': return 3;
		}
		std::cerr << "Invalid key '" << ch << '\'' << std::endl;
		assert(!"Shouldn't be here");
		return 0;
	}

	Rule parse_rule(const std::string_view view)
	{
		using I = typename std::string_view::const_iterator;
		using Match = std::match_results<I>;
		const std::regex re("[xmas][<>](\\d+):(\\w+)");
		if (Match match; std::regex_match(view.begin(), view.end(), match, re))
		{
			return { key_from_char(view[0]), view[1], std::stoll(match[1].str()), match[2].str() };
		}
		std::cerr << "Unable to parse rule for '" << view << '\'' << std::endl;
		assert(!"Shouldn't be here");
		return {};
	}

	struct Workflow
	{
		std::string name;
		std::vector<Rule> rules;
	};

	Workflow parse_workflow(const std::string_view view)
	{
		using Match = std::match_results<typename std::string_view::const_iterator>;
		const std::regex re("(\\w+)\\{(.*)\\}");
		if (Match match; std::regex_match(view.begin(), view.end(), match, re))
		{
			std::string name = match[1].str();
			const std::string rules_string = match[2].str();
			const std::vector<std::string_view> views = aoc::split(rules_string, ',');
			assert(!views.empty());
			std::vector<Rule> rules;
			for (std::size_t i = 0; i < views.size() - 1; ++i)
			{
				rules.push_back(parse_rule(views[i]));
			}
			rules.emplace_back(0, '\0', 0, std::string(views.back()));
			return { std::move(name), std::move(rules) };
		}
		std::cout << "Unable to parse workflow: '" << view << '\'' << std::endl;
		assert(!"Shouldn't be here");
		return {};
	}

	struct Input
	{
		std::unordered_map<std::string, Workflow> workflows;
		std::vector<Part> parts;
	};

	Input read_input(std::istream& is)
	{
		Input input;
		for (std::string line; std::getline(is, line) && !line.empty();)
		{
			Workflow workflow = parse_workflow(line);
			input.workflows[workflow.name] = std::move(workflow);
		}

		for (std::string line; std::getline(is, line);)
		{
			input.parts.push_back(parse_part(line));
		}
		return input;
	}

	bool check_rule(const Rule& rule, const Part& part)
	{
		switch (rule.op)
		{
		case '<': return part[rule.key] < rule.value;
		case '>': return part[rule.key] > rule.value;
		}
		return false;
	}

	bool is_accepted(const std::unordered_map<std::string, Workflow>& workflows, const std::string& name, const Part& part)
	{
		if (name == "A") return true;
		if (name == "R") return false;
		const std::vector<Rule>& rules = workflows.at(name).rules;
		assert(!rules.empty());
		for (std::size_t i = 0; i < rules.size(); ++i)
		{
			if (check_rule(rules[i], part)) return is_accepted(workflows, rules[i].destination, part);
		}
		return is_accepted(workflows, rules.back().destination, part);
	}

	S64 part_one(const Input& input)
	{
		S64 result = 0;
		for (const Part& part : input.parts)
		{
			if (is_accepted(input.workflows, "in", part))
			{
				result = std::accumulate(part.begin(), part.end(), result);
			}
		}
		return result;
	}

	struct Range
	{
		S64 lo;
		S64 hi;

		std::strong_ordering operator<=>(const Range&) const = default;
	};

	using Ranges = std::array<Range, 4>;

	std::pair<Ranges, Ranges> split(const Rule& rule, const Ranges& ranges)
	{
		Ranges t = ranges;
		Ranges f = ranges;
		switch (rule.op)
		{
		case '<':
			{
				if (ranges[rule.key].lo < rule.value)
				{
					if (ranges[rule.key].hi < rule.value)
					{
						f[rule.key] = Range{0, 0};
					}
					else
					{
						t[rule.key].hi = rule.value - 1;
						f[rule.key].lo = rule.value;
					}
				}
				else
				{
					t[rule.key] = Range{0, 0};
				}
			}
			break;
		case '>':
			{
				if (ranges[rule.key].lo > rule.value)
				{
					f[rule.key] = Range{0, 0};
				}
				else
				{
					if (ranges[rule.key].hi > rule.value)
					{
						t[rule.key].lo = rule.value + 1;
						f[rule.key].hi = rule.value;
					}
					else
					{
						t[rule.key] = Range{0, 0};
					}
				}
			}
			break;
		}
		return { t, f };
	}

	S64 product(const Ranges& ranges)
	{
		S64 result = 1;
		for (const Range& range : ranges)
		{
			if (range.lo == 0) return 0;
			result *= (range.hi - range.lo + 1);
		}
		return result;
	}

	S64 count_combinations(const std::unordered_map<std::string, Workflow>& workflows, const std::string& name, Ranges ranges)
	{
		if (name == "A") return product(ranges);
		if (name == "R") return 0;
		S64 combinations = 0;
		const std::vector<Rule>& rules = workflows.at(name).rules;
		assert(!rules.empty());
		for (std::size_t i = 0; i < rules.size() - 1; ++i)
		{
			const std::pair<Ranges, Ranges> result = split(rules[i], ranges);
			if (result.first[rules[i].key].lo != 0) combinations += count_combinations(workflows, rules[i].destination, result.first);
			ranges = result.second;
		}
		return combinations + count_combinations(workflows, rules.back().destination, ranges);
	}

	S64 part_two(const Input& input)
	{
		const Ranges ranges = {Range{1, 4000}, Range{1, 4000}, Range{1, 4000}, Range{1, 4000}};
		return count_combinations(input.workflows, "in", ranges);
	}
}

TEST_CASE("parse parts, rules and workflows", "[parsing]")
{
	{
		const Part part = parse_part("{x=123,m=456,a=789,s=234}");
		REQUIRE(part == Part{123, 456, 789, 234});
	}
	{
		const Rule rule = parse_rule("a<2006:qkq");
		REQUIRE(rule.key == 2);
		REQUIRE(rule.op == '<');
		REQUIRE(rule.value == 2006);
		REQUIRE(rule.destination == "qkq");
	}
	{
		const Workflow workflow = parse_workflow("px{a<2006:qkq,m>2090:A,rfg}");
		REQUIRE(workflow.name == "px");
		REQUIRE(workflow.rules.size() == 3);
		REQUIRE(workflow.rules.back().destination == "rfg");
	}
}

TEST_CASE("split ratings", "[splitting]")
{
	{
		const Rule rule{0, '<', 2301, ""};
		const Ranges input{Range{800, 3000}, Range{1, 4000}, Range{1, 4000}, Range{1, 4000}};
		const std::pair<Ranges, Ranges> result = split(rule, input);
		REQUIRE(result.first == Ranges{Range{800, 2300}, Range{1, 4000}, Range{1, 4000}, Range{1, 4000}});
		REQUIRE(result.second == Ranges{Range{2301, 3000}, Range{1, 4000}, Range{1, 4000}, Range{1, 4000}});
	}
	{
		const Rule rule{0, '<', 2301, ""};
		const Ranges input{Range{800, 2000}, Range{1, 4000}, Range{1, 4000}, Range{1, 4000}};
		const std::pair<Ranges, Ranges> result = split(rule, input);
		REQUIRE(result.first == Ranges{Range{800, 2000}, Range{1, 4000}, Range{1, 4000}, Range{1, 4000}});
		REQUIRE(result.second == Ranges{Range{0, 0}, Range{1, 4000}, Range{1, 4000}, Range{1, 4000}});
	}
	{
		const Rule rule{0, '<', 2301, ""};
		const Ranges input{Range{2400, 4000}, Range{1, 4000}, Range{1, 4000}, Range{1, 4000}};
		const std::pair<Ranges, Ranges> result = split(rule, input);
		REQUIRE(result.first == Ranges{Range{0, 0}, Range{1, 4000}, Range{1, 4000}, Range{1, 4000}});
		REQUIRE(result.second == Ranges{Range{2400, 4000}, Range{1, 4000}, Range{1, 4000}, Range{1, 4000}});
	}
	{
		const Rule rule{0, '>', 2301, ""};
		const Ranges input{Range{800, 3000}, Range{1, 4000}, Range{1, 4000}, Range{1, 4000}};
		const std::pair<Ranges, Ranges> result = split(rule, input);
		REQUIRE(result.first == Ranges{Range{2302, 3000}, Range{1, 4000}, Range{1, 4000}, Range{1, 4000}});
		REQUIRE(result.second == Ranges{Range{800, 2301}, Range{1, 4000}, Range{1, 4000}, Range{1, 4000}});
	}
	{
		const Rule rule{0, '>', 2301, ""};
		const Ranges input{Range{800, 2000}, Range{1, 4000}, Range{1, 4000}, Range{1, 4000}};
		const std::pair<Ranges, Ranges> result = split(rule, input);
		REQUIRE(result.first == Ranges{Range{0, 0}, Range{1, 4000}, Range{1, 4000}, Range{1, 4000}});
		REQUIRE(result.second == Ranges{Range{800, 2000}, Range{1, 4000}, Range{1, 4000}, Range{1, 4000}});
	}
	{
		const Rule rule{0, '>', 2301, ""};
		const Ranges input{Range{2400, 4000}, Range{1, 4000}, Range{1, 4000}, Range{1, 4000}};
		const std::pair<Ranges, Ranges> result = split(rule, input);
		REQUIRE(result.first == Ranges{Range{2400, 4000}, Range{1, 4000}, Range{1, 4000}, Range{1, 4000}});
		REQUIRE(result.second == Ranges{Range{0, 0}, Range{1, 4000}, Range{1, 4000}, Range{1, 4000}});
	}
}

SOLVE
{
	const Input input = read_input(std::cin);

	std::cout << part_one(input) << std::endl;
	std::cout << part_two(input) << std::endl;
}
