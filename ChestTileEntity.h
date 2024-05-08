#pragma once
#include "TileEntity.h"
#include "Container.h"
#include "StorageWindow.h"

class ChestTileEntity : public TileEntity
{
public:
	ChestTileEntity(TileEntityStruct* tileEntityRef_);
	virtual ~ChestTileEntity();

	// Uses base tile entities updating
	//virtual void update() override;

	// Uses base tile entities rendering
	//virtual void render() override;
	//virtual void renderAt(lost::Vector2D pos) override;
	
	virtual void mouseInteractFunction() override;
protected:
	Container* m_Storage;
	int m_Width = 0;
	int m_Height = 0;
	StorageWindow* m_StorageWindow = nullptr;
};

