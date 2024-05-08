#include "BoxWindow.h"
#include "Lost.h"

BoxWindow::BoxWindow(lost::Bound2D bounds)
	: UIWindow(bounds)
{
	m_NBoxData = {};
	m_NBoxData.leftSize = 10;
	m_NBoxData.rightSize = 6;
	m_NBoxData.topSize = 10;
	m_NBoxData.bottomSize = 4;
	m_NBoxData.bounds = m_Bounds;
	m_NBoxData.scale = 6.0f;
	m_NBoxData.imageSize = { 24, 24 };
	m_NBoxData.texture = lost::getImageID("NBoxBase");

	m_ToastHeight = 7 * m_NBoxData.scale;
}

BoxWindow::BoxWindow(lost::Bound2D bounds, lost::NBoxData nBoxData, int toastHeight)
	: UIWindow(bounds)
{
	m_NBoxData = nBoxData;
	m_ToastHeight = toastHeight;
}

BoxWindow::~BoxWindow()
{
}

void BoxWindow::update()
{
	lost::Vector2D mousePos = lost::mousePos();
	if (lost::Bound2D{ m_Bounds.x, m_Bounds.y, m_Bounds.w, (float)m_ToastHeight }.inBounds(mousePos) || m_Dragging)
	{
		if (lost::mouseTapped(0))
		{
			m_Dragging = true;
			m_DraggingStart = lost::mousePos();
			m_WindowDraggingStart = { m_Bounds.x, m_Bounds.y };
		}

		if (!lost::mouseDown(0))
		{
			m_Dragging = false;
		}
		else if (m_Dragging)
		{
			lost::Vector2D dragOffset = lost::mousePos() - m_DraggingStart;
			m_Bounds.x = m_WindowDraggingStart.x + dragOffset.x;
			m_Bounds.y = m_WindowDraggingStart.y + dragOffset.y;
		}
	}

	selected = m_Bounds.inBounds(mousePos) || m_Dragging;
}

void BoxWindow::render()
{
	m_NBoxData.bounds = m_Bounds;
	lost::useImage(m_NBoxData.texture);
	lost::renderNBox(m_NBoxData);
	lost::renderTextPro(m_WindowName, 
		{ 13.0f * m_NBoxData.scale + m_Bounds.x, (4.0f) * m_NBoxData.scale + m_Bounds.y }, 
		0.1f * m_NBoxData.scale,
		LOST_TEXT_ALIGN_LEFT,
		LOST_TEXT_ALIGN_MIDDLE
	);
}

void BoxWindow::setName(std::string name)
{
	m_WindowName = name;
}

void BoxWindow::setType(std::string type, bool maxOne_)
{
	windowType = type;
	maxOne = maxOne_;
}
