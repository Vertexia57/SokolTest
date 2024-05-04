#pragma once
#include "BoxWindow.h"
#include "Container.h"

class StorageWindow : public BoxWindow
{
public:
	StorageWindow(lost::Bound2D bounds);
	StorageWindow(lost::Bound2D bounds, lost::NBoxData nBoxData, int toastHeight);
	virtual ~StorageWindow();

	virtual void update() override;
	virtual void render() override;

	virtual void bindContainer(Container* refContainer_, int width, int height, int slotMin, int slotMax);
protected:
	float m_SlotSize = 64.0f;

	lost::IntVector2D m_HighlightedSlot = { -1, -1 };

	int m_ContainerWidth = 1;
	int m_ContainerHeight = 1;
	int m_SlotMin = 0;
	int m_SlotMax = 0;
	Container* refContainer = nullptr;
};

