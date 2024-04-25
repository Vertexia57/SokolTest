#pragma once
#include "Tile.h"
#include "TileManager.h"
#include "Generator.h"

class Chunk
{
public:
	Chunk(lost::IntVector2D chunkPos, int width, int height);
	~Chunk();

	Tile* getTile(int x, int y);
	Tile* getLocalTile(int x, int y);
	void setTile(TileRefStruct* tile, int x, int y);

	void generateChunk(Generator* generator);

	void addTileEntity(TileEntity* tileEntity, lost::Vector2D position);
	void destroyTileEntity(TileEntity* tileEntity);

	void renderTiles(lost::Bound2D renderBounds);
	void renderTileEntities(lost::Bound2D renderBounds);
	void renderBorders();

private:
	lost::IntVector2D chunkCoord;
	int m_Width, m_Height;
	std::vector<Tile*> m_Tiles;
	std::vector<TileEntity*> m_TileEntities;
};