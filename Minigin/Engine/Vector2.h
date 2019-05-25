#pragma once
namespace Minigin
{
	struct Vector2
	{
		Vector2(float x, float y);
		Vector2();
		Vector2(const Vector2& other);
		Vector2(Vector2&& other);

		float x;
		float y;

		float Magnitude() const;
		float SqrMagnitude() const;

		float Distance(const Vector2& other) const;
		float SqrDistance(const Vector2& other) const;

		float Normalize();
		Vector2 Normalized() const;

		float Dot(const Vector2& other) const;
		float Cross(const Vector2& other) const;

		bool Equals(const Vector2& other, float e = FLT_EPSILON) const;

		//
		bool operator==(const Vector2& other) const;
		bool operator!=(const Vector2& other) const;



		//
		Vector2& operator=(const Vector2& other);
		Vector2& operator=(Vector2&& other);

		Vector2 operator*(const Vector2& other) const;
		Vector2 operator*(float other) const;
		Vector2 operator*(int other) const;

		Vector2 operator+(const Vector2& other) const;
		Vector2 operator+(float other) const;
		Vector2 operator+(int other) const;

		Vector2 operator-(const Vector2& other) const;
		Vector2 operator-(float other) const;
		Vector2 operator-(int other) const;

		Vector2 operator/(const Vector2& other) const;
		Vector2 operator/(float other) const;
		Vector2 operator/(int other) const;

		//
		Vector2& operator*=(const Vector2& other);
		Vector2& operator*=(float other);
		Vector2& operator*=(int other);

		Vector2& operator+=(const Vector2& other);
		Vector2& operator+=(float other);
		Vector2& operator+=(int other);

		Vector2& operator-=(const Vector2& other);
		Vector2& operator-=(float other);
		Vector2& operator-=(int other);

		Vector2& operator/=(const Vector2& other);
		Vector2& operator/=(float other);
		Vector2& operator/=(int other);






		static const Vector2 Zero;
		static const Vector2 Up;
		static const Vector2 Right;
		static const Vector2 One;
	};
}