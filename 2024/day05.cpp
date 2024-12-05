#include "aoc.h"
#include "aoc/string_helpers.h"

#include <algorithm>
#include <cassert>
#include <charconv>
#include <iostream>
#include <ranges>
#include <string>
#include <unordered_map>
#include <vector>

namespace
{
	struct Input
	{
		std::unordered_map<int, std::vector<int>> rules;
		std::vector<std::vector<int>> pages;
	};

	std::pair<int, int> parse_rule(const std::string_view sv)
	{
		const std::vector<std::string_view> parts = aoc::split(sv, '|');
		assert(parts.size() == 2);
		int first;
		std::from_chars(parts[0].data(), parts[0].data() + parts[0].size(), first);
		int second;
		std::from_chars(parts[1].data(), parts[1].data() + parts[1].size(), second);
		return { first, second };
	}

	Input read_input(std::istream& input)
	{
		Input result;

		std::string line;
		while (std::getline(input, line))
		{
			if (line.empty())
			{
				break;
			}
			const auto [first, second] = parse_rule(line);
			result.rules[first].push_back(second);
		}

		for (auto& items : std::views::values(result.rules))
		{
			std::ranges::sort(items);
		}

		while (std::getline(input, line))
		{
			const std::vector<std::string_view> parts = aoc::split(line, ',');
			std::vector<int> page;
			std::ranges::transform(parts, std::back_inserter(page), [](const std::string_view sv)
				{
					int value;
					std::from_chars(sv.data(), sv.data() + sv.size(), value);
					return value;
				});
			result.pages.push_back(std::move(page));
		}

		return result;
	}

	bool is_ordered(const std::unordered_map<int, std::vector<int>>& rules, const std::vector<int>& pages)
	{
		for (std::size_t i = 0; i < pages.size(); ++i)
		{
			for (std::size_t j = i + 1; j < pages.size(); ++j)
			{
				if (const auto iter = rules.find(pages[i]);
					iter == rules.end() || std::ranges::find(iter->second, pages[j]) == iter->second.end())
				{
					return false;
				}
			}
		}
		return true;
	}

	int part_one(const Input& input)
	{
		int result = 0;
		for (const std::vector<int>& updates : input.pages)
		{
			if (is_ordered(input.rules, updates))
			{
				const std::size_t half = updates.size() / 2;
				result += updates[half];
			}
		}
		return result;
	}

	enum class Mark : std::uint8_t
	{
		unmarked,
		temporary,
		permanent,
	};

	bool visit(const int update, const std::unordered_map<int, std::vector<int>>& rules, std::unordered_map<int, Mark>& marks, std::vector<int>& list)
	{
		if (marks[update] == Mark::permanent)
		{
			return true;
		}

		if (marks[update] == Mark::temporary)
		{
			return false; // cycle
		}

		marks[update] = Mark::temporary;

		if (const auto iter = rules.find(update); iter != rules.end())
		{
			for (const int next : iter->second)
			{
				visit(next, rules, marks, list);
			}
		}

		marks[update] = Mark::permanent;
		list.insert(list.begin(), update);
		return true;
	}

	std::vector<int> sort(const std::unordered_map<int, std::vector<int>>& rules, const std::vector<int>& updates)
	{
		std::vector<int> result;
		result.reserve(updates.size());
		std::unordered_map<int, Mark> marks;
		for (const int update : updates)
		{
			marks[update] = Mark::unmarked;
		}

		while (true)
		{
			const auto iter = std::ranges::find_if(marks, [](const auto& pair)
				{
					return pair.second != Mark::permanent;
				});
			if (iter == marks.end())
			{
				break;
			}

			if (!visit(iter->first, rules, marks, result))
			{
				return {};
			}
		}
		return result;
	}

	std::vector<int> order_update(const std::unordered_map<int, std::vector<int>>& rules, std::vector<int> updates)
	{
		std::ranges::sort(updates);
		std::unordered_map<int, std::vector<int>> subset;
		for (const int update : updates)
		{
			if (const auto iter = rules.find(update); iter != rules.end())
			{
				std::vector<int> intersection;
				std::ranges::set_intersection(iter->second, updates, std::back_inserter(intersection));
				subset[iter->first] = std::move(intersection);
			}
		}

		return sort(subset, updates);
	}

	int part_two(const Input& input)
	{
		int result = 0;
		for (const std::vector<int>& updates : input.pages)
		{
			if (is_ordered(input.rules, updates))
			{
				continue;
			}

			const std::vector<int> ordered = order_update(input.rules, updates);
			if (ordered.empty())
			{
				continue;
			}
			const std::size_t half = ordered.size() / 2;
			result += ordered[half];
		}
		return result;
	}
}

SOLVE
{
	const Input input = read_input(std::cin);

	std::cout << part_one(input) << '\n';
	std::cout << part_two(input) << '\n';
}