#pragma once
#include "TileEntity.h"
#include "StorageWindow.h"

class HubTileEntity : public TileEntity
{
public:
	HubTileEntity(TileEntityStruct* tileEntityRef_, uint32_t rotation = 0);
	virtual ~HubTileEntity();

	// Uses base tile entities updating
	//virtual void update() override;

	// Uses base tile entities rendering
	//virtual void render() override;
	//virtual void renderAt(lost::Vector2D pos) override;

	// Returns an empty item, cannot extract from the hub
	virtual Item extractItem(int count, bool output = true);
	// Overrides the normal tile entity insert code to change the max stack size
	virtual void insertItem(Item& item);
protected:
	StorageWindow* m_StorageWindow = nullptr;
};

