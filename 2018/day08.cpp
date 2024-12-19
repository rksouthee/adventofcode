#include "aoc.h"
#include "aoc/core.h"

#include <iostream>
#include <numeric>
#include <span>
#include <vector>

namespace
{
	std::vector<S64> read_input(std::istream& in)
	{
		using I = std::istream_iterator<S64>;
		return { I{in}, I{} };
	}

	struct Node
	{
		std::vector<Node> children;
		std::span<const S64> metadata;
	};

	std::span<const S64> add_nodes(std::vector<Node>& nodes, std::span<const S64> values)
	{
		if (values.empty()) return values;
		const S64 num_children = values[0];
		const S64 num_metadata = values[1];
		values = values.subspan(2);
		Node& node = nodes.emplace_back();
		node.children.reserve(num_children);
		for (S64 i = 0; i < num_children; ++i)
		{
			values = add_nodes(node.children, values);
		}
		node.metadata = values.subspan(0, num_metadata);
		return values.subspan(num_metadata);
	}

	Node make_tree(const std::span<const S64> values)
	{
		Node root;
		add_nodes(root.children, values);
		return root;
	}

	S64 sum_metadata(const Node& node)
	{
		S64 result = 0;
		for (const Node& child : node.children)
		{
			result += sum_metadata(child);
		}
		return std::accumulate(node.metadata.begin(), node.metadata.end(), result);
	}

	S64 part_one(const Node& root)
	{
		return sum_metadata(root.children.front());
	}

	S64 get_node_value(const Node& node)
	{
		if (node.children.empty())
		{
			return std::accumulate(node.metadata.begin(), node.metadata.end(), 0ll);
		}

		S64 result = 0;
		for (S64 i : node.metadata)
		{
			if (i == 0 || i > std::ssize(node.children)) continue;
			result += get_node_value(node.children[i - 1]);
		}
		return result;
	}

	S64 part_two(const Node& root)
	{
		return get_node_value(root.children.front());
	}
}

SOLVE
{
	const std::vector<S64> input = read_input(std::cin);
	const Node root = make_tree(input);

	std::cout << part_one(root) << '\n';
	std::cout << part_two(root) << '\n';
}