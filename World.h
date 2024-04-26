#pragma once
#include "Tile.h"
#include "TileEntity.h"
#include "Chunk.h"
#include <array>
#include "Generator.h"
#include "Entity.h"

class World
{
public:
	World();
	~World();

	void worldInit();

	void update(lost::Bound2D renderBounds);
	void render(lost::Bound2D renderBounds);

	void createChunk(int x);

	void addTileEntity(TileEntity* tileEntity, float x, float y);
	void destroyTileEntity(TileEntity* tileEntity);

	// Checks if the area covered by bounds has any tiles already taking the layers specified
	bool checkCanPlace(lost::Bound2D bounds, std::array<bool, 3> layers);
	// Checks if the area covered by bounds are all stable
	bool checkStable(lost::Bound2D bounds);

	void setTile(TileRefStruct* tile, int x, int y);
	void updateTileConnections(int x, int y);
	void updateTileNeighbors(int x, int y);

	void addEntity(Entity* entity);

	Tile* getTileAt(int x, int y);

	int worldMaxX = 0;
	int worldMinX = 0;
private:
	Tile* m_BorderAir = nullptr;

	std::map<int, Chunk*> m_Chunks;
	std::vector<Entity*> m_Entities;

	int chunkWidth = 32;
	int chunkHeight = 96;

	Generator* worldGenerator;
};

extern World* world;