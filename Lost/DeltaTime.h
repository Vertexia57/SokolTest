#pragma once

#include <chrono>
#include <vector>

namespace lost
{

	extern std::chrono::milliseconds currentMillis;
	extern std::chrono::milliseconds oldMillis;
	extern double deltaTime;

	extern std::chrono::milliseconds pCurrentMillis;
	extern std::chrono::milliseconds pOldMillis;
	extern double processTime;

	// Recalculates the delta time, using the previous time this function was ran as the "OldMillis"
	extern void recalcDeltaTime();

	
	extern void startProcessTime();
	// Recalculates the processing time, using the previous time this function was ran as the "OldMillis"
	extern void calcProcessTime(std::string processName = "noname");
}