#include "stdafx.h"
#include "Color.h"
#include <SDL.h>

using namespace Engine;

Color3& Color3::operator=(const Color3& other)
{
	r = other.r;
	g = other.g;
	b = other.b;
	return *this;
}

Color3& Color3::operator=(Color3&& other)
{
	r = std::move(other.r);
	g = std::move(other.g);
	b = std::move(other.b);
	return *this;
}

Color3::Color3(const Color3& other)
{
	r = other.r;
	g = other.g;
	b = other.b;
}

Color3::Color3(Color3&& other)
{
	r = std::move(other.r);
	g = std::move(other.g);
	b = std::move(other.b);
}

Color3 Color3::operator*(const Color3& other) const
{
	return Color3(r * other.r, g * other.g, b * other.b);
}

Color3 Color3::operator+(const Color3& other) const
{
	return Color3(r + other.r, g + other.g, b - other.b);
}

Color3 Color3::operator-(const Color3& other) const
{
	return Color3(r - other.r, g - other.g, b - other.b);
}

Color3 Color3::operator/(const Color3& other) const
{
	return Color3(r / other.r, g / other.g, b / other.b);
}

Color3& Color3::operator*=(const Color3& other)
{
	r *= other.r;
	g *= other.g;
	b *= other.b;
	return *this;
}

Color3& Color3::operator+=(const Color3& other)
{
	r += other.r;
	g += other.g;
	b += other.b;
	return *this;
}

Color3& Color3::operator-=(const Color3& other)
{
	r -= other.r;
	g -= other.g;
	b -= other.b;
	return *this;
}

Color3& Color3::operator/=(const Color3& other)
{
	r /= other.r;
	g /= other.g;
	b /= other.b;
	return *this;
}

SDL_Color Color3::SDLColor() const
{
	SDL_Color res{};
	res.r = r;
	res.g = g;
	res.b = b;
	res.a = 255;
	return res;
}

Color3::operator struct SDL_Color() const
{
	return SDLColor();
}

//////////
//Color 4
//////////
Color4::Color4(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
	: r(r)
	, g(g)
	, b(b)
	, a(a)
{
}

Color4::Color4(const Color3& rgb, uint8_t a)
	: Color4(rgb.r, rgb.g, rgb.b, a)
{
}

Color4::Color4(Color3&& rgb, uint8_t a)
	: rgb(std::move(rgb))
	, alpha(a)
{
}

Color4& Color4::operator=(const Color4& other)
{
	r = other.r;
	g = other.g;
	b = other.b;
	a = other.a;
	return *this;
}

Color4& Color4::operator=(Color4&& other)
{
	r = std::move(other.r);
	g = std::move(other.g);
	b = std::move(other.b);
	a = std::move(other.a);
	return *this;
}

Color4::Color4(const Color4& other)
	: r(other.r)
	, g(other.g)
	, b(other.b)
	, a(other.a)
{
}

Color4::Color4(Color4&& other)
	: r(std::move(other.r))
	, g(std::move(other.g))
	, b(std::move(other.b))
	, a(std::move(other.a))
{
}

Color4 Color4::operator*(const Color4& other) const
{
	Color4 res{ uint8_t(r * other.r), uint8_t(g * other.g), uint8_t(b*other.b), uint8_t(a*other.a) };
	return res;
}

Color4 Color4::operator+(const Color4& other) const
{
	Color4 res{ uint8_t(r + other.r), uint8_t(g + other.g), uint8_t(b + other.b), uint8_t(a + other.a) };
	return res;
}

Color4 Color4::operator-(const Color4& other) const
{
	Color4 res{ uint8_t(r - other.r), uint8_t(g - other.g), uint8_t(b - other.b), uint8_t(a - other.a) };
	return res;
}

Color4 Color4::operator/(const Color4& other) const
{
	Color4 res{ uint8_t(r / other.r), uint8_t(g / other.g), uint8_t(b / other.b), uint8_t(a / other.a) };
	return res;
}

Color4& Color4::operator*=(const Color4& other)
{
	r *= other.r;
	g *= other.g;
	b *= other.b;
	a *= other.a;
	return *this;
}

Color4& Color4::operator+=(const Color4& other)
{
	r += other.r;
	g += other.g;
	b += other.b;
	a += other.a;
	return *this;
}

Color4& Color4::operator-=(const Color4& other)
{
	r -= other.r;
	g -= other.g;
	b -= other.b;
	a -= other.a;
	return *this;
}

Color4& Color4::operator/=(const Color4& other)
{
	r /= other.r;
	g /= other.g;
	b /= other.b;
	a /= other.a;
	return *this;
}

SDL_Color Color4::SDLColor() const
{
	return SDL_Color{ r, g, b, a };
}

Color4::operator struct SDL_Color() const
{
	return SDLColor();
}
