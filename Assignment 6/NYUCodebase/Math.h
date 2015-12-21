#ifndef MATH_H
#define MATH_H

#include <algorithm>
#include <cmath>

inline float lerp(float v0, float v1, float t) {
	return (1.0f - t)*v0 + t*v1;
}

inline float radians(float degrees)
{
	return degrees * 3.14159f / 180.f;
}

template<typename Numeric>
inline Numeric clamp(Numeric value, Numeric min, Numeric max)
{
	return std::max(min, std::min(max, value));
}

inline float toAngle(float x, float y)
{
	return atan2(y, x);
}

#endif // !MATH_H
