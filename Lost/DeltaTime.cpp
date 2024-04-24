#include "DeltaTime.h"

namespace lost
{
	std::chrono::milliseconds currentMillis = static_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch().count());
	std::chrono::milliseconds oldMillis = static_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch().count());
	double deltaTime = 0.0;

	void recalcDeltaTime()
	{
		currentMillis = static_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch().count());
		deltaTime = (currentMillis - oldMillis).count() / 10000.0f;
		oldMillis = currentMillis;
	}

}