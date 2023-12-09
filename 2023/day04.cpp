#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include <cassert>

namespace
{
	std::set<int> read_numbers(const typename std::string::const_iterator f, const typename std::string::const_iterator l)
	{
		using I = std::istream_iterator<int>;
		const std::string str(f, l);
		std::istringstream iss(str);
		return { I{iss}, I{} };
	}

	using Game = std::set<int>;

	Game get_winners(const std::set<int>& winning, const std::set<int>& have)
	{
		std::set<int> winners;
		std::set_intersection(winning.begin(), winning.end(), have.begin(), have.end(), std::inserter(winners, winners.end()));
		return winners;
	}

	int get_points(const Game& game)
	{
		if (game.size() != 0)
		{
			return 1 << (int)(game.size() - 1);
		}
		return 0;
	}

	std::vector<Game> read_input(std::istream& is)
	{
		std::vector<Game> games;
		std::string line;
		while (std::getline(is, line))
		{
			auto iter = std::find(line.begin(), line.end(), ':');
			assert(iter != line.end() && std::next(iter) != line.end());
			++iter;
			const auto pipe = std::find(iter, line.end(), '|');
			games.emplace_back(get_winners(read_numbers(iter, pipe), read_numbers(std::next(pipe), line.end())));
		}
		return games;
	}

	int part_one(const std::vector<Game>& games)
	{
		int sum = 0;
		for (const Game& game : games)
		{
			sum += get_points(game);
		}
		return sum;
	}

	std::size_t part_two(const std::vector<Game>& games)
	{
		std::vector<std::size_t> counts(games.size(), 1);
		for (std::size_t i = 0; i < games.size(); ++i)
		{
			const std::size_t matches = games[i].size();
			/* std::cout << "Card " << i + 1 << " has " << matches << " matching numbers" << std::endl; */
			const std::size_t count = counts[i];
			for (std::size_t j = 0; j < matches && i + 1 + j < games.size(); ++j)
			{
				int index = i + 1 + j;
				/* std::cout << "adding " << count << " copies of card " << index + 1 << std::endl; */
				counts[index] += count;
			}
		}
		return std::accumulate(counts.begin(), counts.end(), 0);
	}
}

extern "C" void solve(std::istream& is)
{
	const std::vector<Game> games = read_input(is);

	std::cout << part_one(games) << std::endl;
	std::cout << part_two(games) << std::endl;
}
