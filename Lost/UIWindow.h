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

	virtual void setPosition(lost::Vector2D pos);

	inline lost::Bound2D& getBounds() { return m_Bounds; };

	std::string windowType = "base";
	bool maxOne = false;
	bool inventoryWindow = false;

	bool selected = false;
	bool toRemove = false;
	int orderValue = 0;
protected:
	lost::Bound2D m_Bounds;
};