#pragma once

#include <chrono>

namespace lost
{

	extern std::chrono::milliseconds currentMillis;
	extern std::chrono::milliseconds oldMillis;
	extern double deltaTime;

	extern void recalcDeltaTime();

}