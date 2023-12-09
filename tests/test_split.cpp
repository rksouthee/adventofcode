#include <aoc/string_helpers.h>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Split string on comma", "[split]")
{
	const std::string str = "1, 23, 4,";
	std::vector<std::string_view> views = aoc::split(str, ',');
	REQUIRE(views.size() == 3);
	REQUIRE(views[0] == "1");
	REQUIRE(views[1] == " 23");
	REQUIRE(views[2] == " 4");
	/* REQUIRE(views[3] == ""); */
}

TEST_CASE("Split a string based on another string", "[split]")
{
	const std::string str = "1, 23, 4";
	const std::string sep = ", ";
	std::vector<std::string_view> parts = aoc::split(str, sep);
	REQUIRE(parts.size() == 3);
	REQUIRE(parts[0] == "1");
	REQUIRE(parts[1] == "23");
	REQUIRE(parts[2] == "4");
}
