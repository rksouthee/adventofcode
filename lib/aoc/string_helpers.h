/**
 * @file
 */

#ifndef AOC_STRING_HELPERS_H
#define AOC_STRING_HELPERS_H

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
	 * @param str The string to convert.
	 * @returns The number if the conversion was successful, otherwise an empty optional.
	 */
	[[nodiscard]]
	std::optional<int> to_int(std::string_view str);

	/**
	 * @brief Convert a string to a number.
	 *
	 * @param str The string to convert.
	 * @returns The number if the conversion was successful, otherwise 0.
	*/
	[[nodiscard]]
	int to_int_safe(std::string_view str);
}

#endif
