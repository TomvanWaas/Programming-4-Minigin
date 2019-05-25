#pragma once
namespace Minigin
{
	struct Rect
	{
		Rect()
			: Rect(0, 0, 1, 1)
		{}
		Rect(float x, float y, float w, float h)
			: x(x)
			, y(y)
			, width(w)
			, height(h)
		{}

		float x = 0;
		float y = 0;
		float width = 1;
		float height = 1;
	};
}