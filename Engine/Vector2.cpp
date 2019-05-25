#include "stdafx.h"
#include "Vector2.h"
#include <math.h>
#include <float.h>
#include "Logger.h"

using namespace Minigin;

const Vector2 Vector2::Zero = Vector2(0, 0);
const Vector2 Vector2::Up = Vector2(0, -1);
const Vector2 Vector2::Right = Vector2(1, 0);
const Vector2 Vector2::One = Vector2(1, 1);


Vector2::Vector2(float x, float y)
	: x(x)
	, y(y)
{
}
Vector2::Vector2()
	: Vector2(0,0)
{
}
Vector2::Vector2(const Vector2& other)
	: x(other.x)
	, y(other.y)
{
}
Vector2::Vector2(Vector2&& other)
	: x(other.x)
	, y(other.y)
{
}


float Vector2::Magnitude() const
{
	return (sqrtf(SqrMagnitude()));
}
float Vector2::SqrMagnitude() const
{
	return (x * x + y * y);
}

float Vector2::Distance(const Vector2& other) const
{
	return (sqrtf(SqrDistance(other)));
}


float Vector2::SqrDistance(const Vector2& other) const
{
	return ((x - other.x)*(x - other.x) + (y - other.y)*(y - other.y));
}

float Vector2::Normalize()
{
	const float mag = Magnitude();
	if (mag == 0.0f)
	{
		Logger::GetInstance().LogError("Vector2::Normalize > Is zerovector");
		return mag;
	}
	const float m = 1 / mag;
	(*this) *= m;
	return mag;
}
Vector2 Vector2::Normalized() const
{
	const float mag = Magnitude();
	if (mag == 0.0f)
	{
		Logger::GetInstance().LogError("Vector2::Normalized > Is zerovector");
		return (*this);
	}
	const float m = 1 / mag;
	return { x*m, y*m };
}

float Vector2::Dot(const Vector2& other) const
{
	return (x*other.x + y * other.y);
}
float Vector2::Cross(const Vector2& other) const
{
	return (x*other.y - y * other.x);
}

bool Vector2::Equals(const Vector2& other, float e) const
{
	return ((x - other.x)*(x - other.x) < e && (y - other.y)*(y - other.y) < e);
}

//
bool Vector2::operator==(const Vector2& other) const
{
	return (abs(x - other.x) <= FLT_EPSILON
		&& abs(y - other.y) <= FLT_EPSILON);
}
bool Vector2::operator!=(const Vector2& other) const
{
	return !((*this) == other);
}

Vector2& Vector2::operator=(const Vector2& other)
{
	x = other.x;
	y = other.y;
	return *this;
}
Vector2& Vector2::operator=(Vector2&& other)
{
	x = other.x;
	y = other.y;
	return *this;
}

//
Vector2 Vector2::operator*(const Vector2& other) const
{
	return { x*other.x, y*other.y };
}
Vector2 Vector2::operator*(float other) const
{
	return { x*other, y*other };
}
Vector2 Vector2::operator*(int other) const
{
	return { x*other, y*other };
}

Vector2 Vector2::operator+(const Vector2& other) const
{
	return { x + other.x, y + other.y };
}
Vector2 Vector2::operator+(float other) const
{
	return { x + other, y + other };
}
Vector2 Vector2::operator+(int other) const
{
	return { x + other, y + other };
}

Vector2 Vector2::operator-(const Vector2& other) const
{
	return { x - other.x, y - other.y };
}
Vector2 Vector2::operator-(float other) const
{
	return { x - other, y - other };
}
Vector2 Vector2::operator-(int other) const
{
	return { x - other, y - other };
}

Vector2 Vector2::operator/(const Vector2& other) const
{
	return { x / other.x, y / other.y };
}
Vector2 Vector2::operator/(float other) const
{
	return { x / other, y / other };
}
Vector2 Vector2::operator/(int other) const
{
	return { x / other, y / other };
}

//
Vector2& Vector2::operator*=(const Vector2& other)
{
	x *= other.x;
	y *= other.y;
	return *this;
}
Vector2& Vector2::operator*=(float other)
{
	x *= other;
	y *= other;
	return *this;
}
Vector2& Vector2::operator*=(int other)
{
	x *= other;
	y *= other;
	return *this;
}

Vector2& Vector2::operator+=(const Vector2& other)
{
	x += other.x;
	y += other.y;
	return *this;
}
Vector2& Vector2::operator+=(float other)
{
	x += other;
	y += other;
	return *this;
}
Vector2& Vector2::operator+=(int other)
{
	x += other;
	y += other;
	return *this;
}

Vector2& Vector2::operator-=(const Vector2& other)
{
	x -= other.x;
	y -= other.y;
	return *this;
}
Vector2& Vector2::operator-=(float other)
{
	x -= other;
	y -= other;
	return *this;
}
Vector2& Vector2::operator-=(int other)
{
	x -= other;
	y -= other;
	return *this;
}

Vector2& Vector2::operator/=(const Vector2& other)
{
	x /= other.x;
	y /= other.y;
	return *this;
}
Vector2& Vector2::operator/=(float other)
{
	const auto f = 1.0f / other;
	x *= f;
	y *= f;
	return *this;
}
Vector2& Vector2::operator/=(int other)
{
	const auto f = 1.0f / other;
	x *= f;
	y *= f;
	return *this;
}