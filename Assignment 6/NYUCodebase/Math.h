#ifndef MATH_H
#define MATH_H

#include <algorithm>

inline float lerp(float v0, float v1, float t) {
	return (1.0f - t)*v0 + t*v1;
}

inline float radians(float degrees)
{
	return degrees * 3.14159f / 180.f;
}

inline float clamp(float value, float min, float max)
{
	return std::max(min, std::min(max, value));
}

#endif // !MATH_H
