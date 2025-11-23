#ifndef AOC_ORBIT_STRUCTURE_H
#define AOC_ORBIT_STRUCTURE_H

#include <utility>

#include <cstddef>

namespace aoc
{
template <typename F, typename N, typename T> T power(T x, F f, N n)
{
    while (n != N{0})
    {
        x = f(x);
        --n;
    }
    return x;
}

template <typename F, typename T> std::size_t distance(T x, const T &y, F f)
{
    std::size_t n = 0;
    while (x != y)
    {
        x = f(x);
        ++n;
    }
    return n;
}

template <typename F, typename T> T collision_point_nonterminating_orbit(const T &x, F f)
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

template <typename F, typename P, typename T> T collision_point(const T &x, F f, P p)
{
    if (!p(x))
    {
        return x;
    }

    T slow = x;
    T fast = f(x);

    while (fast != slow)
    {
        slow = f(slow);
        if (!p(fast))
        {
            return fast;
        }
        fast = f(fast);
        if (!p(fast))
        {
            return fast;
        }
        fast = f(fast);
    }
    return fast;
}

template <typename F, typename T> T convergent_point(T x0, T x1, F f)
{
    while (x0 != x1)
    {
        x0 = f(x0);
        x1 = f(x1);
    }
    return x0;
}

template <typename F, typename T> T connection_point_nonterminating_orbit(const T &x, F f)
{
    return aoc::convergent_point(x, f(collision_point_nonterminating_orbit(x, f)), f);
}

template <typename F, typename P, typename T> T connection_point(const T &x, F f, P p)
{
    const T y = aoc::collision_point(x, f, p);
    if (!p(y))
    {
        return y;
    }
    return aoc::convergent_point(x, f(y), f);
}

template <typename F, typename P, typename T> bool terminating(const T &x, F f, P p)
{
    return !p(aoc::collision_point(x, f, p));
}

template <typename T> struct Orbit_structure
{
    std::size_t handle_size;
    std::size_t cycle_size;
    T connection_point;
};

template <typename F, typename T> Orbit_structure<T> orbit_structure_nonterminating_orbit(const T &x, F f)
{
    T y = aoc::connection_point_nonterminating_orbit(x, f);
    return {aoc::distance(x, y, f), aoc::distance(f(y), y, f), std::move(y)};
}

template <typename F, typename P, typename T> Orbit_structure<T> orbit_structure(const T &x, F f, P p)
{
    const T y = aoc::connection_point(x, f, p);
    const std::size_t handle_size = aoc::distance(x, y, f);
    std::size_t cycle_size = 0;
    if (p(y))
    {
        cycle_size = aoc::distance(f(y), y, f);
    }
    return {handle_size, cycle_size, std::move(y)};
}

template <typename F, typename T> T iterate_nonterminating_orbit(const T &x, const F f, const std::size_t n)
{
    const Orbit_structure<T> structure = aoc::orbit_structure_nonterminating_orbit(x, f);
    if (n < structure.handle_size)
    {
        return aoc::power(x, f, n);
    }

    const std::size_t cycle_offset = (n - structure.handle_size) % (structure.cycle_size + 1);
    return aoc::power(structure.connection_point, f, cycle_offset);
}
} // namespace aoc

#endif
