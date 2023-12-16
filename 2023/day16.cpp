#include "aoc.h"

#include <iostream>
#include <iterator>
#include <string>
#include <unordered_set>
#include <vector>

#include <cstddef>
#include <cstdint>

namespace
{
	enum class Direction
	{
		right = 0,
		down,
		left,
		up,
	};

	struct Beam
	{
		std::uint32_t x;
		std::uint32_t y;
		Direction d;

		operator std::uint32_t() const
		{
			return (x << 15) | (y << 2) | (std::uint32_t)d;
		}
	};

	using Grid = std::vector<std::string>;

	Grid read_input(std::istream& is)
	{
		using I = std::istream_iterator<std::string>;
		return { I{is}, I{} };
	}

	bool can_move(const Grid& grid, const Beam& beam)
	{
		switch (beam.d)
		{
		case Direction::right: return beam.x + 1 < grid[beam.y].size();
		case Direction::down: return beam.y + 1 < grid.size();
		case Direction::left: return beam.x > 0;
		case Direction::up: return beam.y > 0;
		}
		return false;
	}

	class Beam_adder
	{
	private:
		std::vector<Beam> m_beams;
		std::unordered_set<std::uint32_t> m_visited;

	public:
		void add(const Beam& beam)
		{
			const auto u32 = (std::uint32_t)beam;
			if (m_visited.contains(u32)) return;
			m_visited.insert(u32);
			m_beams.push_back(beam);
		}

		Beam pop()
		{
			Beam beam = m_beams.back();
			m_beams.pop_back();
			return beam;
		}

		bool empty() const
		{
			return m_beams.empty();
		}

		std::size_t seen() const
		{
			const std::uint32_t p_mask = (2 << 15) - 1;
			std::unordered_set<std::uint32_t> points;
			for (const std::uint32_t beam : m_visited)
			{
				std::uint32_t y = (beam >> 2) & p_mask;
				std::uint32_t x = (beam >> 17) & p_mask;
				points.emplace(x << 16u | y);
			}
			return points.size();
		}
	};

	void move_right(const Grid& grid, const Beam& beam, Beam_adder& beams)
	{
		switch (grid[beam.y][beam.x])
		{
		case '|':
			{
				beams.add(Beam{beam.x, beam.y, Direction::down});
				beams.add(Beam{beam.x, beam.y, Direction::up});
			}
			break;
		case '/':
			{
				beams.add(Beam{beam.x, beam.y, Direction::up});
			}
			break;
		case '\\':
			{
				beams.add(Beam{beam.x, beam.y, Direction::down});
			}
			break;
		default:
			{
				beams.add(beam);
			}
			break;
		}
	}

	void move_down(const Grid& grid, const Beam& beam, Beam_adder& beams)
	{
		switch (grid[beam.y][beam.x])
		{
		case '-':
			{
				beams.add(Beam{beam.x, beam.y, Direction::right});
				beams.add(Beam{beam.x, beam.y, Direction::left});
			}
			break;
		case '/':
			{
				beams.add(Beam{beam.x, beam.y, Direction::left});
			}
			break;
		case '\\':
			{
				beams.add(Beam{beam.x, beam.y, Direction::right});
			}
			break;
		default:
			{
				beams.add(beam);
			}
			break;
		}
	}

	void move_left(const Grid& grid, const Beam& beam, Beam_adder& beams)
	{
		switch (grid[beam.y][beam.x])
		{
		case '|':
			{
				beams.add(Beam{beam.x, beam.y, Direction::down});
				beams.add(Beam{beam.x, beam.y, Direction::up});
			}
			break;
		case '/':
			{
				beams.add(Beam{beam.x, beam.y, Direction::down});
			}
			break;
		case '\\':
			{
				beams.add(Beam{beam.x, beam.y, Direction::up});
			}
			break;
		default:
			{
				beams.add(beam);
			}
			break;
		}
	}

	void move_up(const Grid& grid, const Beam& beam, Beam_adder& beams)
	{
		switch (grid[beam.y][beam.x])
		{
		case '-':
			{
				beams.add(Beam{beam.x, beam.y, Direction::right});
				beams.add(Beam{beam.x, beam.y, Direction::left});
			}
			break;
		case '/':
			{
				beams.add(Beam{beam.x, beam.y, Direction::right});
			}
			break;
		case '\\':
			{
				beams.add(Beam{beam.x, beam.y, Direction::left});
			}
			break;
		default:
			{
				beams.add(beam);
			}
			break;
		}
	}

	void move(const Grid& grid, Beam beam, Beam_adder& beams)
	{
		switch (beam.d)
		{
		case Direction::right: ++beam.x; move_right(grid, beam, beams); break;
		case Direction::down: ++beam.y; move_down(grid, beam, beams); break;
		case Direction::left: --beam.x; move_left(grid, beam, beams); break;
		case Direction::up: --beam.y; move_up(grid, beam, beams); break;
		}
	}

	std::size_t count_energy(const Grid& grid, const Beam& start)
	{
		Beam_adder beams;
		switch (start.d)
		{
		case Direction::right: move_right(grid, start, beams); break;
		case Direction::down: move_down(grid, start, beams); break;
		case Direction::left: move_left(grid, start, beams); break;
		case Direction::up: move_up(grid, start, beams); break;
		}
		while (!beams.empty())
		{
			Beam beam = beams.pop();
			if (can_move(grid, beam)) move(grid, beam, beams);
		}
		return beams.seen();
	}

	std::size_t part_one(const Grid& grid)
	{
		return count_energy(grid, Beam{0, 0, Direction::right});
	}

	std::size_t part_two(const Grid& grid)
	{
		std::size_t max = 0;

		// top and bottom row
		for (std::uint32_t x = 0; x < grid[0].size(); ++x)
		{
			max = std::max(max, count_energy(grid, Beam{x, 0, Direction::down}));
			max = std::max(max, count_energy(grid, Beam{x, (std::uint32_t)grid.size() - 1, Direction::up}));
		}

		// left and right column
		for (std::uint32_t y = 0; y < grid.size(); ++y)
		{
			max = std::max(max, count_energy(grid, Beam{0, y, Direction::right}));
			max = std::max(max, count_energy(grid, Beam{(std::uint32_t)grid[y].size() - 1, y, Direction::left}));
		}
		return max;
	}
}

SOLVE
{
	const Grid grid = read_input(std::cin);

	std::cout << part_one(grid) << std::endl;
	std::cout << part_two(grid) << std::endl;
}
