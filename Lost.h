#pragma warning(disable:4305)
#pragma warning(disable:4244)
#pragma warning(disable:4267)
#pragma once

#include "Lost/DeltaTime.h"
#include "Lost/ImageManager.h"
#include "Lost/ShaderManager.h"
#include "Lost/Camera.h"
#include "Lost/InputManager.h"
#include "Lost/LuaInit.h"
#include "Lost/FontManager.h"
#include "Lost/UIManager.h"
#include "Lost/RenderExtras.h"
#include "Lost/Graph.h"

namespace lost
{

	void createManagers();
	void destroyManagers();

}

static int min(int a, int b)
{
	return (a > b) ? a : b;
};