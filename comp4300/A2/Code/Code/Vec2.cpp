#include "Vec2.h"

#include <cmath>

Vec2::Vec2(float xin, float yin)
	: x(xin), y(yin)
{
}

bool Vec2::operator==(const Vec2& rhs) const
{
	return (x == rhs.x && y == rhs.y);
}

bool Vec2::operator!=(const Vec2& rhs) const
{
	return !(*this == rhs);
}

Vec2 Vec2::operator+(const Vec2& rhs) const
{
	return Vec2(x + rhs.x, y + rhs.y);
}

Vec2 Vec2::operator-(const Vec2& rhs) const
{
	return Vec2(x - rhs.x, y - rhs.y);
}

Vec2 Vec2::operator/(const float val) const
{
	return Vec2(x / val, y / val);
}

Vec2 Vec2::operator*(const float val) const
{
	return Vec2(x * val, y * val);
}

void Vec2::operator+=(const Vec2& rhs)
{
	x += rhs.x;
	y += rhs.y;
}

void Vec2::operator-=(const Vec2& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
}

void Vec2::operator*=(const float val)
{
	x *= val;
	y *= val;
}

void Vec2::operator/=(const float val)
{
	x /= val;
	y /= val;
}

void Vec2::normalize()
{
	float magnitude = Vec2(0.0f, 0.0f).dist(*this);
	if (magnitude != 0.0f)
	{
		*this /= magnitude;
	}
}

float Vec2::dist(const Vec2& rhs) const
{
	return sqrtf((x - rhs.x) * (x - rhs.x) + (y - rhs.y) * (y - rhs.y));
}
