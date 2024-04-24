#pragma once

#include <chrono>

namespace lost
{

	extern std::chrono::milliseconds currentMillis;
	extern std::chrono::milliseconds oldMillis;
	extern double deltaTime;

	// Recalculates the delta time, using the previous time this function was ran as the "OldMillis"
	extern void recalcDeltaTime();

}