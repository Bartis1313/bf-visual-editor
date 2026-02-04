#pragma once

#include <numbers>
#include <cmath>

namespace math
{
	constexpr float PI = std::numbers::pi_v<float>;
	constexpr float PI_2 = std::numbers::pi_v<float> *2.0f;

	[[nodiscard]] constexpr float DEG2RAD(float degrees) { return degrees * (PI / 180.0f); }
	[[nodiscard]] constexpr float RAD2DEG(float radians) { return radians * (180.0f / PI); }
}