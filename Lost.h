#pragma once

#include "Lost/DeltaTime.h"
#include "Lost/ImageManager.h"
#include "Lost/ShaderManager.h"
#include "Lost/Camera.h"
#include "Lost/InputManager.h"
#include "Lost/LuaInit.h"
#include "Lost/FontManager.h"

namespace lost
{

	void createManagers();
	void destroyManagers();

}

static int min(int a, int b)
{
	return (a > b) ? a : b;
};