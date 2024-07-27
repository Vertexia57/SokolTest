#pragma once
#include "TileEntity.h"
#include "ItemManager.h"
#include "RecipieSelectWindow.h"

class FactoryTileEntity : public TileEntity
{
public:
	FactoryTileEntity(TileEntityStruct* tileEntityRef_, uint32_t rotation = 0);
	virtual ~FactoryTileEntity();

	//virtual void init();

	virtual void update() override;
	virtual void tileUpdate() override;

	void setRecipie(RecipieRefStruct* recipie);

	virtual void mouseInteractFunction() override;
protected:
	RecipieRefStruct* m_SetRecipie = nullptr;
	std::string m_CraftingGroup = "base";

	RecipieSelectWindow* m_RecipieWindow = nullptr;
};

