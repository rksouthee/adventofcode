#include <aoc/algorithm.h>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <string_view>
#include <string>
#include <vector>

namespace
{
	constexpr auto max_value = std::numeric_limits<std::uint64_t>::max();

	using Pair = std::pair<std::uint64_t, std::uint64_t>;

	struct First_less
	{
		bool operator()(const Pair& x, const Pair& y) const
		{
			return x.first < y.first;
		}
	};

	template <typename I, typename O>
	O read_numbers(I f_i, I l_i, O f_o)
	{
		while (f_i != l_i)
		{
			std::uint64_t x{};
			f_i = aoc::parse_nonnegative_integer(f_i, l_i, x);
			*f_o++ = x;
			if (f_i != l_i)
			{
				++f_i;
			}
		}
		return f_o;
	}

	union Range
	{
		struct
		{
			std::uint64_t destination;
			std::uint64_t source;
			std::uint64_t length;
		};
		std::uint64_t values[3];

		Range() = default;
		Range(std::uint64_t d, std::uint64_t s, std::uint64_t l) :
			destination{d},
			source{s},
			length{l}
		{
		}
	};

	Range read_range(const std::string_view line)
	{
		Range range;
		read_numbers(line.begin(), line.end(), &range.values[0]);
		return range;
	}

	std::vector<std::uint64_t> read_seeds(const std::string_view line)
	{
		const auto iter = std::find(line.begin(), line.end(), ':');
		std::vector<std::uint64_t> seeds;
		read_numbers(std::next(iter, 2), line.end(), std::back_inserter(seeds));
		return seeds;
	}

	using Map = std::vector<Range>;

	Map complete_map(const Map& map)
	{
		Map complete;
		std::uint64_t source = 0;
		for (const Range& range : map)
		{
			if (range.source != source)
			{
				complete.emplace_back(source, source, range.source - source);
			}
			complete.push_back(range);
			source = range.source + range.length;
		}
		complete.emplace_back(source, source, max_value - source);
		return complete;
	}

	Map read_map(std::istream& is)
	{
		Map map;
		std::string line;
		std::getline(is, line);
		while (std::getline(is, line) && !line.empty())
		{
			map.push_back(read_range(line));
		}
		std::sort(map.begin(), map.end(), [] (const Range& x, const Range& y) -> bool
			  {
				  return x.source < y.source;
			  });
		return complete_map(map);
	}

	struct Almanac
	{
		std::vector<std::uint64_t> seeds;
		Map maps[7];
	};

	Almanac read_almanac(std::istream& is)
	{
		Almanac almanac;
		std::string line;
		std::getline(is, line);
		almanac.seeds = read_seeds(line);
		std::getline(is, line);
		for (std::size_t i = 0; i < std::size(almanac.maps); ++i)
		{
			almanac.maps[i] = read_map(is);
		}
		return almanac;
	}

	bool in_counted_range(std::uint64_t start, std::uint64_t length, std::uint64_t value)
	{
		return value >= start && (value - start) < length;
	}

	void resolve(const Map& map, std::uint64_t start, std::uint64_t length, std::vector<Pair>& out)
	{
		auto iter = map.begin();
		while (true)
		{
			if (in_counted_range(iter->source, iter->length, start))
			{
				const std::uint64_t offset = start - iter->source;
				if (offset + length <= iter->length)
				{
					out.emplace_back(iter->destination + offset, length);
					return;
				}
				const std::uint64_t this_length = iter->length - offset;
				out.emplace_back(iter->destination + offset, this_length);
				length -= this_length;
				start += this_length;
			}
			++iter;
		}
	}

	std::uint64_t get_min_location(const Almanac& almanac, std::vector<Pair> input)
	{
		std::sort(input.begin(), input.end(), First_less{});
		for (const Map& map : almanac.maps)
		{
			std::vector<Pair> temp;
			for (const Pair& p : input)
			{
				resolve(map, p.first, p.second, temp);
			}
			input = std::move(temp);
			std::sort(input.begin(), input.end(), First_less{});
		}
		return input.front().first;
	}

	std::uint64_t part_one(const Almanac& almanac)
	{
		std::vector<Pair> input;
		for (const std::uint64_t seed : almanac.seeds)
		{
			input.emplace_back(seed, 1u);
		}
		return get_min_location(almanac, input);
	}

	std::uint64_t part_two(const Almanac& almanac)
	{
		std::vector<Pair> input;
		const std::vector<std::uint64_t>& seeds = almanac.seeds;
		for (std::size_t i = 0; i < seeds.size() / 2; ++i)
		{
			const std::uint64_t start = seeds[2 * i];
			const std::uint64_t length = seeds[2 * i + 1];
			input.emplace_back(start, length);
		}
		return get_min_location(almanac, input);
	}
}

extern "C" void solve(std::istream& is)
{
	const Almanac& almanac = read_almanac(is);
	std::cout << part_one(almanac) << std::endl;
	std::cout << part_two(almanac) << std::endl;
}
