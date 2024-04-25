#pragma once
#include "Tile.h"
#include "TileEntity.h"
#include "Chunk.h"
#include <array>
#include "Generator.h"

class World
{
public:
	World();
	~World();

	void worldInit();

	void render(lost::Bound2D renderBounds);

	void createChunk(int x);

	void addTileEntity(TileEntity* tileEntity, float x, float y);
	void destroyTileEntity(TileEntity* tileEntity);

	// Checks if the area covered by bounds has any tiles already taking the layers specified
	bool checkCanPlace(lost::Bound2D bounds, std::array<bool, 3> layers);

	void setTile(TileRefStruct* tile, int x, int y);
	void updateTileConnections(int x, int y);
	void updateTileNeighbors(int x, int y);

	Tile* getTileAt(int x, int y);

private:
	Tile* m_BorderAir = nullptr;
	std::map<int, Chunk*> m_Chunks;
	int chunkWidth = 32;
	int chunkHeight = 128;

	Generator* worldGenerator;
};

