#include <aoc/string_helpers.h>

#include <aoc/algorithm.h>

namespace
{
	struct String_view_adder
	{
		using I = std::string_view::const_iterator;

		std::vector<std::string_view>& parts;  // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)

		void operator()(const I f, const I l) const // NOLINT
		{
			parts.emplace_back(f, l);
		}
	};
}

namespace aoc
{
	std::vector<std::string_view> split(const std::string_view str, const char sep)
	{
		std::vector<std::string_view> result;
		split(str.begin(), str.end(), sep, String_view_adder{result});
		return result;
	}

	std::vector<std::string_view> split(const std::string_view str, const std::string_view sep)
	{
		std::vector<std::string_view> result;
		split(str.begin(), str.end(), sep.begin(), sep.end(), String_view_adder{result});
		return result;
	}
}
