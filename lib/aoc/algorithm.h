/**
 * @file
 */

#ifndef AOC_ALGORITHM_H
#define AOC_ALGORITHM_H

#include <algorithm>

#include <cctype>

namespace aoc
{
	/**
	 * @brief Find the first item in a range that is not equal to a given value.
	 *
	 * @tparam I InputIterator.
	 * @tparam T The value type of @p T.
	 * @param f The iterator pointing to the beginning of the range.
	 * @param l The iterator pointing to the end of the range.
	 * @param x The value used for searching.
	 * @return The iterator with value not equal to @p x or @p l if no such element exists.
	 */
	template <typename I, typename T>
	I find_not(I f, I l, const T& x)
	{
		while (f != l && *f == x)
			++f;
		return f;
	}

	/**
	 * @brief
	 *
	 * @tparam I BidirectionalIterator.
	 * @tparam P UnaryPredicate that takes a value type of I.
	 * @param f The iterator pointing to the beginning of the range.
	 * @param l The iterator pointing to the end of the range.
	 * @param p The unary predicate.
	 * @return An iterator one past the item that satisfies the predicate or @p f if no such element exists.
	 */
	template <typename I, typename P>
	I find_if_backward(I f, I l, P p)
	{
		while (true)
		{
			if (l == f) return f;
			I t = std::prev(l);
			if (p(*t)) return l;
			l = t;
		}
	}

	/**
	 * @brief
	 *
	 * @tparam I ForwardIterator
	 * @tparam T Regular
	 * @tparam F BinaryProcedure taking two iterators (I)
	 * @param f_i An iterator to the beginning of the range.
	 * @param l_i An iterator to the end of the range.
	 * @param x The value used as the delimiter.
	 * @param fn Function that is passed a sub range.
	 * @return The function object.
	 */
	template <typename I, typename T, typename F>
	F split(I f_i, I l_i, const T& x, F fn)
	{
		while (true)
		{
			f_i = aoc::find_not(f_i, l_i, x);
			if (f_i == l_i) return fn;
			I t = std::find(std::next(f_i), l_i, x);
			fn(f_i, t);
			f_i = t;
		}
	}

	/**
	 * @brief Find the first position where two ranges have a mismatch element.
	 *
	 * @tparam I0 InputIterator
	 * @tparam I1 InputIterator
	 * @param f_i0 An iterator to the beginning of the first range.
	 * @param l_i0 An iterator to the end of the first range.
	 * @param f_i1 An iterator to the beginning of the second range.
	 * @param l_i1 An iterator to the end of the second range.
	 * @returns A pair of iterators where they are the first mismatching elements or the end of either of the
	 * ranges.
	 *
	 * @todo This should accept a Relation and default to equality.
	 */
	template <typename I0, typename I1>
	std::pair<I0, I1> find_mismatch(I0 f_i0, I0 l_i0, I1 f_i1, I1 l_i1)
	{
		while (f_i0 != l_i0 && f_i1 != l_i1 && *f_i0 == *f_i1)
		{
			++f_i0;
			++f_i1;
		}
		return {f_i0, f_i1};
	}

	/**
	 * @brief Search for a range inside another range.
	 *
	 * @tparam I0 ForwardIterator
	 * @tparam I1 ForwardIterator
	 * @param f_i0 An iterator pointing to the beginning of the range to examine (the haystack).
	 * @param l_i0 An iterator pointing to the end of the the the range to examine (the haystack).
	 * @param f_i1 An iterator pointing to the beginning of the range to search (the needle).
	 * @param l_i1 An iterator pointing to the beginning of the range to search (the needle).
	 * @returns A pair of iterators specifying the range containing [f_i1, l_i1). If the search wasn't found then
	 * it returns the empty range [l_i0, l_i0).
	 */
	template <typename I0, typename I1>
	std::pair<I0, I0> search(I0 f_i0, I0 l_i0, I1 f_i1, I1 l_i1)
	{
		while (true)
		{
			I0 f = std::find(f_i0, l_i0, *f_i1);
			if (f == l_i0) return {f, f};
			std::pair<I0, I1> p = aoc::find_mismatch(std::next(f), l_i0, std::next(f_i1), l_i1);
			if (p.second == l_i1) return {f, p.first};
			if (p.first == l_i0) return {l_i0, l_i0};
			++f_i0;
		}
	}

	/**
	 * @brief Split a range using another range.
	 *
	 * @tparam I0 ForwardIterator
	 * @tparam I1 ForwardIterator
	 * @tparam F Function
	 * @param f_i0 An iterator pointing to the beginning of the range to examine (the haystack).
	 * @param l_i0 An iterator pointing to the end of the the the range to examine (the haystack).
	 * @param f_i1 An iterator pointing to the beginning of the range to search (the needle).
	 * @param l_i1 An iterator pointing to the beginning of the range to search (the needle).
	 * @param fn A function that takes a range that has been split.
	 */
	template <typename I0, typename I1, typename F>
	F split(I0 f_i0, I0 l_i0, I1 f_i1, I1 l_i1, F fn)
	{
		while (true)
		{
			std::pair<I0, I0> p = aoc::search(f_i0, l_i0, f_i1, l_i1);
			fn(f_i0, p.first);
			f_i0 = p.second;
			if (f_i0 == l_i0) return fn;
		}
	}

	/**
	 * @brief Parse a non-negative integer.
	 *
	 * @tparam I InputIterator
	 * @tparam T Integer
	 * @param f An iterator pointing to the beginning of the range.
	 * @param l An iterator pointing to the end of the range.
	 * @param value The value that will be updated from the range.
	 * @returns An iterator pointing to the end of the range after parsing the integer.
	 */
	template <typename I, typename T>
	I parse_nonnegative_integer(I f, I l, T& value)
	{
		while (f != l && std::isdigit(*f))
		{
			value = 10 * value + (*f - '0');
			++f;
		}
		return f;
	}

	/**
	 * @brief Copy the range [f_i, l_i) where each output element is unique and the count.
	 *
	 * @tparam I ForwardIterator
	 * @tparam O OutputIterator
	 * @param f_i An iterator pointing to the beginning of the input range.
	 * @param l_i An iterator pointing to the end of the input range.
	 * @param f_o An iterator pointing to the beginning of the output range.
	 * @returns An iterator pointing to the output location.
	 *
	 * @pre std::is_sorted(f_i, l_i)
	 */
	template <typename I, typename O>
	O unique_copy_with_count(I f_i, I l_i, O f_o)
	{
		using N = typename std::iterator_traits<I>::difference_type;
		while (f_i != l_i)
		{
			N n = 1;
			I start = f_i++;
			while (f_i != l_i && *f_i == *start)
			{
				++n;
				++f_i;
			}
			*f_o++ = std::make_pair(*start, n);
		}
		return f_o;
	}
}

#endif
