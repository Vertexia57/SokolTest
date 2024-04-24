#pragma once
#include "Tile.h"

class Chunk
{
public:
	Chunk(lost::IntVector2D chunkPos, int width, int height);
	~Chunk();

	Tile* getTile(int x, int y);
	void setTile(Tile* tile, int x, int y);

	void addTileEntity(TileEntity* tileEntity, lost::Vector2D position);
	void destroyTileEntity(TileEntity* tileEntity);

	void render();

private:
	lost::IntVector2D chunkCoord;
	int m_Width, m_Height;
	std::vector<Tile*> m_Tiles;
	std::vector<TileEntity*> m_TileEntities;
};