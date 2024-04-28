#include "DeltaTime.h"
#include "../SokolReference.h"

namespace lost
{
	std::chrono::milliseconds currentMillis = static_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch().count());
	std::chrono::milliseconds oldMillis = static_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch().count());
	double deltaTime = 0.0;

	std::chrono::milliseconds pCurrentMillis = static_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch().count());
	std::chrono::milliseconds pOldMillis = static_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch().count());
	double processTime = 0.0;

	void recalcDeltaTime()
	{
		currentMillis = static_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch().count());
		deltaTime = (currentMillis - oldMillis).count() / 10000.0f;
		oldMillis = currentMillis;
	}

	void startProcessTime()
	{
		pCurrentMillis = static_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch().count());
		pOldMillis = pCurrentMillis;
		processTime = 0.0;
	}

	void calcProcessTime(std::string processName)
	{
		pCurrentMillis = static_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch().count());
		processTime = (pCurrentMillis - pOldMillis).count() / 10000.0f;
		pOldMillis = pCurrentMillis;
		ImGui::Text((processName + ": %f").c_str(), (float)processTime);
	}
}