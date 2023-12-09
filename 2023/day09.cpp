#include <catch2/catch_test_macros.hpp>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <string>
#include <vector>

namespace
{
	int predict_forward(std::vector<int> xs)
	{
		int value = xs.back();
		std::vector<int> ys(xs.size(), 0);
		for (std::size_t i = 0; i < xs.size(); ++i)
		{
			std::adjacent_difference(xs.begin() + i, xs.end(), ys.begin() + i);
			if (std::all_of(ys.begin() + i + 1, ys.end(), [](int x){return x == 0;}))
			{
				return value;
			}
			value += ys.back();
			std::swap(xs, ys);
		}
		return value;
	}

	int predict_backward(std::vector<int> xs)
	{
		int sign = -1;
		int value = xs.front();
		std::vector<int> ys(xs.size(), 0);
		for (std::size_t i = 0; i < xs.size(); ++i)
		{
			std::adjacent_difference(xs.begin() + i, xs.end(), ys.begin() + i);
			if (std::all_of(ys.begin() + i + 1, ys.end(), [](int x){return x == 0;}))
			{
				return value;
			}
			value += sign * ys[i + 1];
			sign *= -1;
			std::swap(xs, ys);
		}
		return value;
	}

	int sum_predictions(const std::vector<std::vector<int>>& histories, int (*predict)(std::vector<int>))
	{
		int sum = 0;
		for (const std::vector<int>& history : histories)
		{
			sum += predict(history);
		}
		return sum;
	}

	[[nodiscard]] int part_one(const std::vector<std::vector<int>>& histories)
	{
		return sum_predictions(histories, predict_forward);
	}

	[[nodiscard]] int part_two(const std::vector<std::vector<int>>& histories)
	{
		return sum_predictions(histories, predict_backward);
	}

	std::vector<int> read_history(const std::string& line)
	{
		using I = std::istream_iterator<int>;
		std::istringstream iss(line);
		return { I{iss}, I{} };
	}

	std::vector<std::vector<int>> read_input(std::istream& is)
	{
		std::vector<std::vector<int>> histories;
		for (std::string line; std::getline(is, line);)
		{
			histories.push_back(read_history(line));
		}
		return histories;
	}
}

TEST_CASE("predict forward", "[part_one]")
{
	REQUIRE(predict_forward({0, 3, 6, 9, 12, 15}) == 18);
	REQUIRE(predict_forward({1, 3, 6, 10, 15, 21}) == 28);
	REQUIRE(predict_forward({10, 13, 16, 21, 30, 45}) == 68);
}

TEST_CASE("predict backward", "[part_two]")
{
	REQUIRE(predict_backward({0, 3, 6, 9, 12, 15}) == -3);
	REQUIRE(predict_backward({1, 3, 6, 10, 15, 21}) == 0);
	REQUIRE(predict_backward({10, 13, 16, 21, 30, 45}) == 5);
}

extern "C" void solve(std::istream& is)
{
	const std::vector<std::vector<int>> histories = read_input(is);

	std::cout << part_one(histories) << std::endl;
	std::cout << part_two(histories) << std::endl;
}
