#pragma once
#include "Lost/UIWindow.h"
#include "Lost/ImageManager.h"
#include "TileManager.h"

class HUDWindow : public UIWindow
{
public:
	HUDWindow(lost::Bound2D bounds);
	virtual ~HUDWindow();

	virtual void update() override;
	virtual void render() override;
protected:
	TextureID m_TopLeftUITexID;
	bool m_Building = false;
	int m_BuildingSlot = 0;
	int m_HoveredBuildingSlot = -1;
	int m_SelectedBuildingSlot = -1;
	TileEntityStruct* m_SelectedBuilding = nullptr;
	bool m_CanPlaceBuilding = false;
};

