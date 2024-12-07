/**
 * @file
 */

#ifndef AOC_STRING_HELPERS_H
#define AOC_STRING_HELPERS_H

#include <charconv>
#include <concepts>
#include <optional>
#include <string_view>
#include <vector>

namespace aoc
{
	/**
	 * @brief Split a string using a delimiter.
	 *
	 * @param str The string to split.
	 * @param sep The delimiter to split on.
	 * @returns A list of string views that have been split.
	 */
	[[nodiscard]]
	std::vector<std::string_view> split(std::string_view str, char sep);

	/**
	 * @brief Split a string using a separator.
	 *
	 * @param str The string to split.
	 * @param sep The separator to split on.
	 * @returns A list of string views that have been split.
	 */
	[[nodiscard]]
	std::vector<std::string_view> split(std::string_view str, std::string_view sep);

	/**
	 * @brief Convert a string to a number.
	 * 
	 * @tparam T The type of number to convert to.
	 * @param str The string to convert.
	 * @returns The number if the conversion was successful, otherwise an empty optional.
	 */
	template <std::integral T>
	[[nodiscard]]
	std::optional<T> convert(const std::string_view str)
	{
		const char* const end = str.data() + str.size();
		if (T result; std::from_chars(str.data(), end, result).ptr == end)
		{
			return result;
		}
		return std::nullopt;
	}

	/**
	 * @brief Convert a string to a number.
	 *
	 * @tparam T The type of number to convert to.
	 * @param str The string to convert.
	 * @returns The number if the conversion was successful, otherwise a value initialized instance of T.
	 */
	template <std::integral T>
	[[nodiscard]]
	T convert_unguarded(const std::string_view str)
	{
		return convert<T>(str).value_or(T{});
	}
}

#endif
