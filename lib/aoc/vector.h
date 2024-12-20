/**
 * @file
 */

#pragma once

#include "aoc/core.h"

#include <compare>

namespace aoc
{
	struct Vector2
	{
		S64 x;
		S64 y;

		std::strong_ordering operator<=>(const Vector2&) const = default;

		static const Vector2 east;
		static const Vector2 south_east;
		static const Vector2 south;
		static const Vector2 south_west;
		static const Vector2 west;
		static const Vector2 north_west;
		static const Vector2 north;
		static const Vector2 north_east;
	};

	Vector2 operator+(const Vector2& lhs, const Vector2& rhs);
	Vector2& operator+=(Vector2& lhs, const Vector2& rhs);
	Vector2 operator-(const Vector2& lhs, const Vector2& rhs);
	Vector2& operator-=(Vector2& lhs, const Vector2& rhs);
	Vector2 operator-(const Vector2& rhs);
	Vector2 operator*(const Vector2& lhs, S64 rhs);
	Vector2 operator*(S64 lhs, const Vector2& rhs);

	S64 manhattan_distance(const Vector2& p, const Vector2& q);
}
