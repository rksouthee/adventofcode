#include "aoc.h"
#include "aoc/core.h"

#include <iostream>
#include <string>
#include <regex>
#include <vector>

namespace
{
	struct Game
	{
		S64 players;
		S64 last_marble;
	};

	Game read_input(std::istream& in)
	{
		const std::regex re(R"((\d+) players; last marble is worth (\d+) points)");
		Game game;
		std::string line;
		std::getline(in, line);
		std::smatch match;
		std::regex_match(line, match, re);
		game.players = std::stoll(match[1]);
		game.last_marble = std::stoll(match[2]);
		return game;
	}

	struct Node
	{
		Node* prev;
		Node* next;
		S64 value;
	};

	class Free_list
	{
	private:
		std::vector<Node> m_nodes;
		Node* m_free_list = nullptr;

	public:
		void reserve(const S64 n)
		{
			m_nodes.reserve(n);
		}

		Node* allocate(Node* prev, Node* next, const S64 value)
		{
			if (m_free_list == nullptr)
			{
				return &m_nodes.emplace_back(prev, next, value);
			}
			Node* node = m_free_list;
			m_free_list = m_free_list->next;
			node->prev = prev;
			node->next = next;
			node->value = value;
			return node;
		}

		Node* free(Node* node)
		{
			node->prev->next = node->next;
			node->next->prev = node->prev;
			Node* tail = node->next;
			node->next = m_free_list;
			m_free_list = node;
			return tail;
		}
	};

	S64 get_high_score(const Game& game)
	{
		Node zero = { &zero, &zero, 0 };
		const S64 num_nodes = game.last_marble - (game.last_marble / 23);
		Free_list nodes;
		nodes.reserve(num_nodes);
		std::vector<S64> scores(game.players);
		Node* cursor = &zero;
		for (S64 marble = 1; marble <= game.last_marble; ++marble)
		{
			if (marble % 23 == 0)
			{
				const S64 player = marble % game.players;
				scores[player] += marble;
				for (S64 i = 0; i < 7; ++i) cursor = cursor->prev;
				scores[player] += cursor->value;
				cursor = nodes.free(cursor);
			}
			else
			{
				cursor = cursor->next;
				Node* new_node = nodes.allocate(cursor, cursor->next, marble);
				cursor->next->prev = new_node;
				cursor->next = new_node;
				cursor = new_node;
			}
		}
		return std::ranges::max(scores);
	}

	S64 part_one(const Game& game)
	{
		return get_high_score(game);
	}

	S64 part_two(const Game& game)
	{
		return get_high_score({ game.players, game.last_marble * 100 });
	}
}

SOLVE
{
	const Game game = read_input(std::cin);

	std::cout << part_one(game) << '\n';
	std::cout << part_two(game) << '\n';
}