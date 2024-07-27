#pragma once
#include "BoxWindow.h"
#include "ItemManager.h"

class FactoryTileEntity;

class RecipieSelectWindow : public BoxWindow
{
public:
	RecipieSelectWindow(lost::Bound2D bounds, FactoryTileEntity* factoryRef, std::string& craftingGroup);
	RecipieSelectWindow(lost::Bound2D bounds, lost::NBoxData nBoxData, int toastHeight, FactoryTileEntity* factoryRef, std::string& craftingGroup);
	virtual ~RecipieSelectWindow();

	virtual void update() override;
	virtual void render() override;
protected:
	float m_SlotSize = 64.0f;

	int m_ContainerWidth = 5;
	int m_ContainerHeight = 1;
	lost::IntVector2D m_HighlightedSlot = { -1, -1 };

	// A reference to the crafting group that it's going to display the options of
	const std::map<std::string, RecipieRefStruct*>* m_CraftingGroupRef = nullptr;

	FactoryTileEntity* m_FactoryRef = nullptr;
	std::string m_CraftingGroup;
};

