#pragma once
#include "Lost/UIWindow.h"
#include "Lost/RenderExtras.h"

class BoxWindow : public UIWindow
{
public:
	BoxWindow(lost::Bound2D bounds);
	BoxWindow(lost::Bound2D bounds, lost::NBoxData nBoxData, int toastHeight);
	virtual ~BoxWindow();

	virtual void update() override;
	virtual void render() override;

	virtual void setName(std::string name);

protected:
	lost::NBoxData m_NBoxData;
	int m_ToastHeight = 0;
	bool m_Dragging = false;
	lost::Vector2D m_DraggingStart = { 0.0f, 0.0f };
	lost::Vector2D m_WindowDraggingStart = { 0.0f, 0.0f };
	std::string m_WindowName = "";
};

