#include <aoc/algorithm.h>

#include <catch2/catch_test_macros.hpp>

#include <array>

TEST_CASE("Find an element not in an array", "[algorithm]")
{
	int xs[] = { 1, 3, 1, 4, 1, 5 };
	const int* const first = std::begin(xs);
	const int* const last = std::end(xs);
	const int* iter = aoc::find_not(first, last, 2);
	REQUIRE(iter == first);
	REQUIRE(*first == 1);
	iter = aoc::find_not(first, last, 3);
	REQUIRE(iter == first);
	REQUIRE(*first == 1);
	iter = aoc::find_not(first, last, 1);
	REQUIRE(iter == first + 1);
	REQUIRE(*iter == 3);
}

TEST_CASE("Find the last element in a range with a predicate", "[algorithm]")
{
	int xs[] = { 1, 3, 1, 4, 1, 5 };
	int* ptr = aoc::find_if_backward(std::begin(xs), std::end(xs), [](int x) { return x % 2 == 0; });
	REQUIRE(ptr == xs + 4);
	ptr = aoc::find_if_backward(std::begin(xs), std::end(xs), [](int x) { return x > 4; });
	REQUIRE(ptr == xs + 6);
	ptr = aoc::find_if_backward(std::begin(xs), std::end(xs), [](int x) { return x > 20; });
	REQUIRE(ptr == xs);
}

TEST_CASE("Find a mismatching element", "[algorithm]")
{
	int xs[] = { 1, 3, 1, 4, 1, 5 };
	int ys[] = { 6, 7, 1, 3, 1 };
	std::pair<int*, int*> p = aoc::find_mismatch(xs, xs + 6, ys, ys + 5);
	REQUIRE(p.first == xs);
	REQUIRE(p.second == ys);
	p = aoc::find_mismatch(xs, xs + 6, ys + 2, ys + 5);
	REQUIRE(p.first == xs + 3);
	REQUIRE(p.second == ys + 5);
}

TEST_CASE("Search for a range inside another range", "[algorithm]")
{
	const std::string haystack = "Some books are to be tasted";
	const std::string needle = "to";

	using I = typename std::string::const_iterator;
	using P = std::pair<I, I>;
	P p = aoc::search(haystack.begin(), haystack.end(), needle.begin(), needle.end());
	REQUIRE(p.first == haystack.begin() + 15);
	REQUIRE(p.second == haystack.begin() + 17);
}

TEST_CASE("Parse an integer from a string", "[algorithm]")
{
	const std::string str = "123 ";
	int value = 0;
	auto iter = aoc::parse_nonnegative_integer(str.begin(), str.end(), value);
	REQUIRE(iter == str.begin() + 3);
	REQUIRE(value == 123);
}

TEST_CASE("Copy the unique elements with the count from a sorted range", "[algorithm]")
{
	int xs[] = { 1, 1, 1, 3, 3, 4, 5, 5, 5, 5 };
	std::pair<int, std::ptrdiff_t> ys[4];
	auto* iter = aoc::unique_copy_with_count(xs, xs + 10, ys);
	REQUIRE(iter == ys + 4);
	std::pair<int, std::ptrdiff_t> expected[] = {{1, 3}, {3, 2}, {4, 1}, {5, 4}};
	REQUIRE(std::equal(ys, ys + 4, expected));
}
