/**
 * @file
 */

#ifndef AOC_STRING_HELPERS_H
#define AOC_STRING_HELPERS_H

#include <string_view>
#include <vector>

namespace aoc
{
	/**
	 * @brief Split a string using a delimiter.
	 *
	 * @param str The string to split.
	 * @param delim The delimiter to split on.
	 * @returns A list of string views that have been split.
	 */
	std::vector<std::string_view> split(std::string_view str, char delim);

	/**
	 * @brief Split a string using a separator.
	 *
	 * @param str The string to split.
	 * @param sep The separator to split on.
	 * @returns A list of string views that have been split.
	 */
	std::vector<std::string_view> split(const std::string_view str, std::string_view sep);
}

#endif
