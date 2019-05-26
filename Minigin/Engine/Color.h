#pragma once

//To Disable Warning about the nameless struct in the union
#pragma warning (push)
#pragma warning (disable:4201)

struct SDL_Color;
namespace Engine
{

	struct Color3
	{
		Color3(uint8_t r = 255, uint8_t g = 255, uint8_t b = 255)
			: r(r)
			, g(g)
			, b(b)
		{}
		~Color3() = default;

		Color3& operator=(const Color3& other);
		Color3& operator=(Color3&& other);
		Color3(const Color3& other);
		Color3(Color3&& other);


		uint8_t r;
		uint8_t g;
		uint8_t b;


		Color3 operator*(const Color3& other) const;
		Color3 operator+(const Color3& other) const;
		Color3 operator-(const Color3& other) const;
		Color3 operator/(const Color3& other) const;
		Color3& operator*=(const Color3& other);
		Color3& operator+=(const Color3& other);
		Color3& operator-=(const Color3& other);
		Color3& operator/=(const Color3& other);

		SDL_Color SDLColor() const;
		operator SDL_Color() const;

	};

	struct Color4
	{
		//Variables
		union
		{
			struct
			{
				uint8_t r;
				uint8_t g;
				uint8_t b;
				uint8_t a;
			};
			struct
			{
				Color3 rgb;
				uint8_t alpha;
			};
			uint8_t rgba[4];
		};

		//
		Color4(uint8_t r = 255, uint8_t g = 255, uint8_t b = 255, uint8_t a = 255);
		Color4(const Color3& rgb, uint8_t a = 255);
		Color4(Color3&& rgb, uint8_t a = 255);
		~Color4() = default;

		Color4& operator=(const Color4& other);
		Color4& operator=(Color4&& other);
		Color4(const Color4& other);
		Color4(Color4&& other);




		Color4 operator*(const Color4& other) const;
		Color4 operator+(const Color4& other) const;
		Color4 operator-(const Color4& other) const;
		Color4 operator/(const Color4& other) const;
		Color4& operator*=(const Color4& other);
		Color4& operator+=(const Color4& other);
		Color4& operator-=(const Color4& other);
		Color4& operator/=(const Color4& other);

		SDL_Color SDLColor() const;
		operator SDL_Color() const;

	};
}

#pragma warning (pop)