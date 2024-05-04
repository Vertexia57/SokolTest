#pragma once
#include "Transform2D.h"
#include <string>

// Base class for all UI windows
class UIWindow
{
public:
	UIWindow(int x, int y, int w, int h);
	UIWindow(float x, float y, float w, float h);
	UIWindow(lost::Bound2D bounds_);
	virtual ~UIWindow();

	virtual void update();
	virtual void render();

	std::string windowType = "base";
protected:
	lost::Bound2D m_Bounds;
};