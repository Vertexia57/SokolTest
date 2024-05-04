#include "UIWindow.h"
#include "../SokolReference.h"
#include "InputManager.h"
#include "RenderExtras.h"

UIWindow::UIWindow(int x, int y, int w, int h)
	: m_Bounds(x, y, w, h)
{
}

UIWindow::UIWindow(float x, float y, float w, float h)
	: m_Bounds(x, y, w, h)
{
}

UIWindow::UIWindow(lost::Bound2D bounds_)
	: m_Bounds(bounds_)
{
}

UIWindow::~UIWindow()
{
}

void UIWindow::update()
{
}

void UIWindow::render()
{
}
