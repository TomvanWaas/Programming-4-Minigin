#pragma once

#ifndef PI
#define PI 3.14159265359f
#endif
#ifndef RADTODEG
#define RADTODEG 57.2957795f
#endif
#ifndef DEGTORAD
#define DEGTORAD 0.0174532925f 
#endif

namespace Minigin
{
	namespace Mathf
	{
		inline float Min(float a, float b)
		{
			return (a > b) ? b : a;
		}
		inline float Max(float a, float b)
		{
			return (a < b) ? b : a;
		}
		inline float Clamp(float value, float min, float max)
		{
			return Max(min, Min(value, max));
		}
		inline float Clamp01(float value)
		{
			return Clamp(value, 0.0f, 1.0f);
		}








	}
}