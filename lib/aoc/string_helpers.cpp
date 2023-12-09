#include <aoc/string_helpers.h>

#include <aoc/algorithm.h>

namespace
{
	struct String_view_adder
	{
		using I = typename std::string_view::const_iterator;

		std::vector<std::string_view>& parts;

		void operator()(const I f, const I l) const
		{
			parts.emplace_back(f, l);
		}
	};
}

namespace aoc
{
	std::vector<std::string_view> split(const std::string_view sv, const char delim)
	{
		std::vector<std::string_view> result;
		aoc::split(sv.begin(), sv.end(), delim, String_view_adder{result});
		return result;
	}

	std::vector<std::string_view> split(const std::string_view str, const std::string_view sep)
	{
		std::vector<std::string_view> result;
		aoc::split(str.begin(), str.end(), sep.begin(), sep.end(), String_view_adder{result});
		return result;
	}
}
