#include "aoc/vector.h"

namespace aoc
{
	const Vector2 Vector2::east = { .x = 1, .y = 0 };
	const Vector2 Vector2::south_east = { .x = 1, .y = 1 };
	const Vector2 Vector2::south = { .x = 0, .y = 1 };
	const Vector2 Vector2::south_west = { .x = -1, .y = 1 };
	const Vector2 Vector2::west = { .x = -1, .y = 0 };
	const Vector2 Vector2::north_west = { .x = -1, .y = -1 };
	const Vector2 Vector2::north = { .x = 0, .y = -1 };
	const Vector2 Vector2::north_east = { .x = 1, .y = -1 };

	Vector2 operator+(const Vector2& lhs, const Vector2& rhs)
	{
		return { .x = lhs.x + rhs.x, .y = lhs.y + rhs.y };
	}

	Vector2 operator*(const Vector2& lhs, const S64 rhs)
	{
		return { .x = lhs.x * rhs, .y = lhs.y * rhs };
	}
}