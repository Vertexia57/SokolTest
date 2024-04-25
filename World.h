#pragma once
#include "Tile.h"
#include "TileEntity.h"
#include "Chunk.h"
#include <array>a

class World
{
public:
	World();
	~World();

	void render();

	void addTileEntity(TileEntity* tileEntity, float x, float y);
	void destroyTileEntity(TileEntity* tileEntity);

	// Checks if the area covered by bounds has any tiles already taking the layers specified
	bool checkCanPlace(lost::Bound2D bounds, std::array<bool, 3> layers);

	Tile* getTileAt(int x, int y);

private:
	std::map<int, Chunk*> m_Chunks;
};

