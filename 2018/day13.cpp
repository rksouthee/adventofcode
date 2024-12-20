#include "aoc.h"
#include "aoc/vector.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <set>
#include <string>
#include <vector>

using aoc::Vector2;

namespace
{
	using Grid = std::vector<std::string>;

	enum class Turn : U8
	{
		left,
		straight,
		right,
	};

	enum class Direction : U8
	{
		east = '>',
		south = 'v',
		west = '<',
		north = '^',
	};

	Direction from_char(const char ch)
	{
		return static_cast<Direction>(ch);
	}

	char to_char(const Direction direction)
	{
		return static_cast<char>(direction);
	}

	Vector2 from_direction(const Direction direction)
	{
		switch (direction)
		{
		case Direction::east: return Vector2::east;
		case Direction::south: return Vector2::south;
		case Direction::west: return Vector2::west;
		case Direction::north: return Vector2::north;
		}
		return {};
	}

	struct Cart
	{
		Vector2 position;
		Direction direction;
		Turn turn;
	};

	struct Cart_sorter
	{
		bool operator()(const Cart& lhs, const Cart& rhs) const
		{
			return std::tie(lhs.position.y, lhs.position.x) < std::tie(rhs.position.y, rhs.position.x);
		}
	};

	Grid read_input(std::istream& is)
	{
		Grid grid;
		for (std::string line; std::getline(is, line);)
		{
			grid.push_back(line);
		}
		return grid;
	}

	std::vector<Cart> find_carts(const Grid& grid)
	{
		std::vector<Cart> carts;
		for (S64 y = 0; y < std::ssize(grid); ++y)
		{
			for (S64 x = 0; x < std::ssize(grid[y]); ++x)
			{
				switch (grid[y][x])
				{
				case '>':
				{
					carts.emplace_back(Vector2{ x, y }, Direction::east);
					break;
				}
				case 'v':
				{
					carts.emplace_back(Vector2{ x, y }, Direction::south);
					break;
				}
				case '<':
				{
					carts.emplace_back(Vector2{ x, y }, Direction::west);
					break;
				}
				case '^':
				{
					carts.emplace_back(Vector2{ x, y }, Direction::north);
					break;
				}
				}
			}
		}
		return carts;
	}

	void turn_left(Cart& cart)
	{
		switch (cart.direction)
		{
		case Direction::east: cart.direction = Direction::north; break;
		case Direction::south: cart.direction = Direction::east; break;
		case Direction::west: cart.direction = Direction::south; break;
		case Direction::north: cart.direction = Direction::west; break;
		}
	}

	void turn_right(Cart& cart)
	{
		switch (cart.direction)
		{
		case Direction::east: cart.direction = Direction::south; break;
		case Direction::south: cart.direction = Direction::west; break;
		case Direction::west: cart.direction = Direction::north; break;
		case Direction::north: cart.direction = Direction::east; break;
		}
	}

	void turn(Cart& cart)
	{
		if (cart.turn == Turn::left)
		{
			turn_left(cart);
		}
		else if (cart.turn == Turn::right)
		{
			turn_right(cart);
		}
		cart.turn = static_cast<Turn>((static_cast<U8>(cart.turn) + 1) % 3);
	}

	bool tick(const Grid& grid, Cart& cart)
	{
		cart.position += from_direction(cart.direction);
		switch (grid[cart.position.y][cart.position.x])
		{
		case '+':
		{
			turn(cart);
			break;
		}
		case '-':
		{
			assert(cart.direction == Direction::east || cart.direction == Direction::west);
			break;
		}
		case '/':
		{
			switch (cart.direction)
			{
			case Direction::east: cart.direction = Direction::north; break;
			case Direction::south: cart.direction = Direction::west; break;
			case Direction::west: cart.direction = Direction::south; break;
			case Direction::north: cart.direction = Direction::east; break;
			}
			break;
		}
		case '\\':
		{
			switch (cart.direction)
			{
			case Direction::east: cart.direction = Direction::south; break;
			case Direction::south: cart.direction = Direction::east; break;
			case Direction::west: cart.direction = Direction::north; break;
			case Direction::north: cart.direction = Direction::west; break;
			}
			break;
		}
		case '|':
		{
			assert(cart.direction == Direction::south || cart.direction == Direction::north);
			break;
		}
		default:
		{
			return true;
		}
		}
		return false;
	}

	std::vector<Vector2> tick(const Grid& grid, std::vector<Cart>& carts)
	{
		std::set<Vector2> positions;
		for (const Cart& cart : carts)
		{
			positions.insert(cart.position);
		}
		std::vector<S64> to_remove;
		std::vector<Vector2> collisions;
		for (auto iter = carts.begin(); iter != carts.end(); ++iter)
		{
			if (!positions.contains(iter->position)) continue;
			positions.erase(iter->position);
			tick(grid, *iter);
			if (positions.contains(iter->position))
			{
				const auto pred = [iter](const Cart& cart) { return cart.position == iter->position; };
				collisions.push_back(iter->position);
				to_remove.push_back(iter - carts.begin());
				auto other = std::ranges::find_if(carts, pred);
				if (other == iter)
				{
					other = std::ranges::find_if(std::next(iter), carts.end(), pred);
				}
				to_remove.push_back(other - carts.begin());
				positions.erase(iter->position);
			}
			else
			{
				positions.insert(iter->position);
			}
		}
		std::ranges::sort(to_remove, std::greater{});
		for (const S64 index : to_remove)
		{
			assert(index >= 0 && index < std::ssize(carts));
			carts.erase(carts.begin() + index);
		}
		std::ranges::sort(carts, Cart_sorter{});
		return collisions;
	}

	void replace_carts_with_track(Grid& grid, const std::vector<Cart>& carts)
	{
		for (const Cart& cart : carts)
		{
			grid[cart.position.y][cart.position.x] = (cart.direction == Direction::east || cart.direction == Direction::west) ? '-' : '|';
		}
	}

	Vector2 part_one(const Grid& grid, std::vector<Cart> carts)
	{
		std::vector<Vector2> collisions;
		while (collisions.empty())
		{
			collisions = tick(grid, carts);
		}
		return collisions.front();
	}

	Vector2 part_two(const Grid& grid, std::vector<Cart> carts)
	{
		while (carts.size() != 1)
		{
			tick(grid, carts);
		}
		return carts.front().position;
	}
}

SOLVE
{
	Grid grid = read_input(std::cin);
	const std::vector<Cart> carts = find_carts(grid);
	replace_carts_with_track(grid, carts);

	const Vector2 collision = part_one(grid, carts);
	std::cout << collision.x << ',' << collision.y << '\n';

	const Vector2 last = part_two(grid, carts);
	std::cout << last.x << ',' << last.y << '\n';
}