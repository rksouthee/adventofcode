#ifndef AOC_ORBIT_STRUCTURE_H
#define AOC_ORBIT_STRUCTURE_H

#include <utility>

#include <cstddef>

namespace aoc
{
	template <typename F, typename N, typename T>
	T power(T x, F f, N n)
	{
		while (n != N{0})
		{
			x = f(x);
			--n;
		}
		return x;
	}

	template <typename F, typename T>
	std::size_t distance(T x, const T& y, F f)
	{
		std::size_t n = 0;
		while (x != y)
		{
			x = f(x);
			++n;
		}
		return n;
	}

	template <typename F, typename T>
	T collision_point_nonterminating_orbit(const T& x, F f)
	{
		T slow = x;
		T fast = f(x);

		while (fast != slow)
		{
			slow = f(slow);
			fast = f(fast);
			fast = f(fast);
		}
		return fast;
	}

	template <typename F, typename T>
	T convergent_point(T x0, T x1, F f)
	{
		while (x0 != x1)
		{
			x0 = f(x0);
			x1 = f(x1);
		}
		return x0;
	}

	template <typename F, typename T>
	T connection_point_nonterminating_orbit(const T& x, F f)
	{
		return aoc::convergent_point(x, f(collision_point_nonterminating_orbit(x, f)), f);
	}

	template <typename T>
	struct Orbit_structure
	{
		std::size_t handle_size;
		std::size_t cycle_size;
		T connection_point;
	};

	template <typename F, typename T>
	Orbit_structure<T> orbit_structure_nonterminating_orbit(const T& x, F f)
	{
		T y = aoc::connection_point_nonterminating_orbit(x, f);
		return { aoc::distance(x, y, f), aoc::distance(f(y), y, f), std::move(y) };
	}
}

#endif
