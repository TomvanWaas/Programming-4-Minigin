#pragma once
#include <string>

namespace Minigin
{
	struct WindowSettings final
	{
		WindowSettings(float w, float h, const std::string& name)
			: width(w)
			, height(h)
			, name(name)
		{}

		float width;
		float height;
		std::string name;
	};
}