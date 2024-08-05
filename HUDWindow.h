#pragma once
#include "Lost/UIWindow.h"
#include "Lost/ImageManager.h"
#include "TileManager.h"
#include "BuildingTileEntities.h"
#include "Lost/Animator.h"

class HUDWindow : public UIWindow
{
public:
	HUDWindow(lost::Bound2D bounds);
	virtual ~HUDWindow();

	virtual void update() override;
	virtual void render() override;

	inline void setHUDInteraction(bool state) { m_CanInteract = state; };
protected:
	bool m_CanInteract = true;

	TextureID m_TopLeftUITexID;
	bool m_Building = false;
	int m_BuildingSlot = 0;
	uint32_t m_Rotation = 0;
	int m_HoveredBuildingSlot = -1;
	int m_HoveredGroupSlot = -1;
	int m_SelectedBuildingSlot = -1;
	int m_SelectedGroupSlot = -1;
	int m_OldGroupSlot = -1;
	TileEntityStruct* m_SelectedBuilding = nullptr;
	bool m_CanPlaceBuilding = false;

	lost::Animator m_Animator;
	lost::Animation* m_BuildMenuAnim;
	bool m_BuildSubMenuOpen = false;
	lost::Animation* m_BuildSubMenuAnims[4] = { nullptr, nullptr, nullptr, nullptr };
	lost::AnimationChain* m_SubMenuIconAnim;
};

