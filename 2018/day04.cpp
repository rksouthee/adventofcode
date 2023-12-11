#include "aoc.h"

#include <array>
#include <iostream>
#include <numeric>
#include <regex>
#include <string>
#include <unordered_map>

#include <cassert>

namespace
{
	struct Timestamp
	{
		int year;
		int month;
		int day;
		int hour;
		int minute;
	};

	bool operator<(const Timestamp& x, const Timestamp& y)
	{
		return std::tie(x.year, x.month, x.day, x.hour, x.minute) < std::tie(y.year, y.month, y.day, y.hour, y.minute);
	}

	enum class Kind
	{
		begins_shift,
		wakes_up,
		falls_asleep,
	};

	struct Action
	{
		Timestamp timestamp;
		int guard;
		Kind kind;
	};

	Timestamp parse_timestamp(const std::smatch& match)
	{
		Timestamp timestamp;
		timestamp.year = std::stoi(match[1].str());
		timestamp.month = std::stoi(match[2].str());
		timestamp.day = std::stoi(match[3].str());
		timestamp.hour = std::stoi(match[4].str());
		timestamp.minute = std::stoi(match[5].str());
		return timestamp;
	}

	Action parse_action(const std::string& str)
	{
		const std::regex re("\\[(\\d\\d\\d\\d)-(\\d\\d)-(\\d\\d) (\\d\\d):(\\d\\d)\\] (.*)");
		std::smatch match;
		Action action{};
		if (std::regex_match(str, match, re))
		{
			action.timestamp = parse_timestamp(match);
			std::string command_str = match[6].str();
			if (command_str[0] == 'G')
			{
				action.kind = Kind::begins_shift;
				const std::regex guard_re("Guard #(\\d+) begins shift");
				if (std::regex_match(command_str, match, guard_re))
				{
					action.guard = std::stoi(match[1].str());
				}
			}
			else if (command_str[0] == 'f')
			{
				action.kind = Kind::falls_asleep;
			}
			else if (command_str[0] == 'w')
			{
				action.kind = Kind::wakes_up;
			}
			else
			{
				assert(!"Shouldn't be here");
			}
		}
		return action;
	}

	std::vector<Action> read_input(std::istream& is)
	{
		std::vector<Action> actions;
		std::string line;
		while (std::getline(is, line))
		{
			actions.push_back(parse_action(line));
		}
		std::sort(actions.begin(), actions.end(), [] (const Action& x, const Action& y) -> bool
			  {
				  return x.timestamp < y.timestamp;
			  });
		return actions;
	}

	using Sleep_map = std::array<int, 60>;

	std::unordered_map<int, Sleep_map> get_sleep_map(const std::vector<Action>& actions)
	{
		std::unordered_map<int, Sleep_map> guards;
		std::size_t guard = 0;
		for (std::size_t i = 0; i < actions.size(); ++i)
		{
			const Action& action = actions[i];
			switch (action.kind)
			{
			case Kind::begins_shift:
				{
					guard = action.guard;
				}
				break;

			case Kind::falls_asleep:
				{
					/* std::cout << "Guard #" << guard << " falls asleep at minute " << action.timestamp.minute << std::endl; */
				}
				break;

			case Kind::wakes_up:
				{
					/* std::cout << "Guard #" << guard << " wakes up at minute " << action.timestamp.minute << std::endl; */
					assert(actions[i - 1].kind == Kind::falls_asleep);
					const Action& sleep = actions[i - 1];
					for (int minute = sleep.timestamp.minute; minute < action.timestamp.minute; ++minute)
					{
						++guards[guard][minute];
					}
				}
				break;
			}
		}
		return guards;
	}

	struct Sleep_result
	{
		int guard;
		int total_minutes_slept;
		int minute_spent_most_asleep;
		int frequency;
	};

	std::vector<Sleep_result> get_sleep_results(const std::unordered_map<int, Sleep_map>& maps)
	{
		std::vector<Sleep_result> results;
		for (const std::pair<const int, Sleep_map>& p : maps)
		{
			const int total_minutes_slept = std::accumulate(p.second.begin(), p.second.end(), 0);
			const auto minute = std::max_element(p.second.begin(), p.second.end());
			results.emplace_back(p.first, total_minutes_slept, (int)(minute - p.second.begin()), *minute);
		}
		return results;
	}

	template <typename R>
		int get_id(const std::vector<Sleep_result>& sleep_results, const R r)
		{
			const auto iter = std::max_element(sleep_results.begin(), sleep_results.end(), r);
			return iter->guard * iter->minute_spent_most_asleep;
		}

	int part_one(const std::vector<Sleep_result>& sleep_results)
	{
		const auto r = [] (const Sleep_result& x, const Sleep_result& y) -> bool
		{
			return x.total_minutes_slept < y.total_minutes_slept;
		};
		return get_id(sleep_results, r);
	}

	int part_two(const std::vector<Sleep_result>& sleep_results)
	{
		const auto r = [] (const Sleep_result& x, const Sleep_result& y) -> bool
		{
			return x.frequency < y.frequency;
		};
		return get_id(sleep_results, r);
	}
}

SOLVE
{
	const std::vector<Action> actions = read_input(std::cin);
	const std::unordered_map<int, Sleep_map> guard_sleep_maps = get_sleep_map(actions);
	const std::vector<Sleep_result> sleep_results = get_sleep_results(guard_sleep_maps);

	std::cout << part_one(sleep_results) << std::endl;
	std::cout << part_two(sleep_results) << std::endl;
}
