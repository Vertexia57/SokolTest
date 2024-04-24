#pragma once
#include "Tile.h"
#include "TileEntity.h"
#include "Chunk.h"

class World
{
public:
	World();
	~World();

	void render();

	void addTileEntity(TileEntity* tileEntity, float x, float y);
	void destroyTileEntity(TileEntity* tileEntity);

	Tile* getTileAt(int x, int y);

private:
	std::map<int, Chunk*> m_Chunks;
};

