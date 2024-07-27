#pragma once
#include "TileEntity.h"
#include "Container.h"

class Tile;

class OreDrillEntity : public TileEntity
{
public:
	OreDrillEntity(TileEntityStruct* tileEntityRef_, uint32_t rotation = 0);
	virtual ~OreDrillEntity();

	virtual void init();

	virtual void update() override;
	virtual void tileUpdate() override;

	// Uses base tile entities rendering
	//virtual void render() override;
	//virtual void renderAt(lost::Vector2D pos) override;

	//virtual void mouseInteractFunction() override;
protected:
	Container* m_Storage;
	// This is the location the drill will dig at
	Tile* drillTile = nullptr;
	int* drillTileCountRef = nullptr;

	lost::IntVector2D drillLocation = { 0, -1 }; // y < 0 is impossible, use invalid state as init test
	void m_SearchForOres();

	TileEntity* m_LeftEntity = nullptr;
	TileEntity* m_RightEntity = nullptr;

	float m_TimeSinceLastMine = 0.0f;
	float m_MiningTime = 3000.0f;
};

