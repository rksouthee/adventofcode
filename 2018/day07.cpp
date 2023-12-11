#include "aoc.h"

#include <catch2/catch_test_macros.hpp>

#include <array>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

#include <cassert>
#include <cstddef>

namespace
{
	unsigned to_bitset(char ch)
	{
		assert(ch >= 'A' && ch <= 'Z');
		return 1u << (ch - 'A');
	}

	char to_char(unsigned x)
	{
		assert(x != 0);
		int n = 0;
		while (x != 1)
		{
			x /= 2;
			++n;
		}
		return 'A' + n;
	}

	struct Node
	{
		unsigned in_edges;
		unsigned out_edges;
	};

	std::array<Node, 26> read_input(std::istream& is)
	{
		const std::size_t src_index = 5;
		const std::size_t dst_index = 36;
		std::array<Node, 26> nodes = {};
		for (std::string line; std::getline(is, line);)
		{
			assert(line.size() > dst_index);
			char src = line[src_index];
			char dst = line[dst_index];
			nodes[src - 'A'].out_edges |= to_bitset(dst);
			nodes[dst - 'A'].in_edges |= to_bitset(src);
		}
		return nodes;
	}

	struct Worker
	{
		unsigned node;
		unsigned time;
	};

	std::string part_one(std::array<Node, 26> nodes)
	{
		std::priority_queue<unsigned, std::vector<unsigned>, std::greater<unsigned>> queue;
		for (std::size_t i = 0; i < 26; ++i)
		{
			if (nodes[i].in_edges == 0)
			{
				queue.push(i);
			}
		}

		std::string result;
		while (!queue.empty())
		{
			unsigned node = queue.top();
			queue.pop();
			for (unsigned i = 0; i < 26; ++i)
			{
				unsigned bit = 1u << i;
				if (nodes[node].out_edges & bit)
				{
					nodes[i].in_edges &= ~(1u << node);
					if (nodes[i].in_edges == 0)
					{
						queue.push(i);
					}
				}
			}
			result += char('A' + node);
		}
		return result;
	}

	unsigned part_two(std::array<Node, 26> nodes)
	{
		std::priority_queue<unsigned, std::vector<unsigned>, std::greater<unsigned>> queue;
		for (std::size_t i = 0; i < 26; ++i)
		{
			if (nodes[i].in_edges == 0)
			{
				queue.push(i);
			}
		}

		Worker workers[5];
		for (Worker& worker : workers)
		{
			worker.node = nodes.size();
			worker.time = 0;
		}

		unsigned time = 0;
		while (true)
		{
			// assign work to available workers
			for (Worker& worker : workers)
			{
				if (queue.empty()) break;
				unsigned node = queue.top();
				if (worker.node == nodes.size())
				{
					worker.node = node;
					worker.time = time + (60 + 1 + node);
					queue.pop();
				}
			}

			unsigned min_time = -1;
			for (Worker& worker : workers)
			{
				if (worker.node != nodes.size() && worker.time < min_time)
				{
					min_time = worker.time;
				}
			}

			time = min_time;
			for (Worker& worker : workers)
			{
				if (worker.node == nodes.size() || worker.time != min_time) continue;
				for (unsigned i = 0; i < nodes.size(); ++i)
				{
					unsigned bit = 1u << i;
					if (nodes[worker.node].out_edges & bit)
					{
						nodes[i].in_edges &= ~(1u << worker.node);
						if (nodes[i].in_edges == 0)
						{
							queue.push(i);
						}
					}
				}
				worker.node = nodes.size();
			}

			bool finished = true;
			for (const Worker& worker : workers)
			{
				if (worker.node != nodes.size())
				{
					finished = false;
					break;
				}
			}

			if (finished && queue.empty()) return time;
		}
		return time;
	}
}

TEST_CASE("convert char to a bitset", "[7]")
{
	REQUIRE(to_bitset('A') == 0x01);
	REQUIRE(to_bitset('E') == 0x10);
}

TEST_CASE("convert bitset to a char", "[7]")
{
	REQUIRE(to_char(0x01) == 'A');
	REQUIRE(to_char(0x10) == 'E');
	REQUIRE(to_char(0x80) == 'H');
}

SOLVE
{
	/** @todo: Allow passing the number of nodes as a parameter, the examples use fewer nodes */
	std::array<Node, 26> nodes = read_input(std::cin);

	std::cout << part_one(nodes) << std::endl;
	std::cout << part_two(nodes) << std::endl;
}
