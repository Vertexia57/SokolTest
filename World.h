#pragma once
#include "Tile.h"
#include "TileEntity.h"
#include "Chunk.h"
#include <array>
#include "Generator.h"
#include "Entity.h"

struct OutOfBoundsTileEntityStruct
{
	TileEntityStruct* object;
	float x;
	float y;
	float placeX;
	float placeY;
};

class World
{
public:
	World();
	~World();

	void worldInit();

	void update(lost::Bound2D renderBounds);
	void render(lost::Bound2D renderBounds);

	void createChunk(int x);
	void finishCreation(int x);
	void checkOutOfBoundEntities();

	void createTileEntity(TileEntityStruct* tileEntity, float x, float y);
	void addTileEntity(TileEntity* tileEntity, float x, float y);
	void destroyTileEntity(TileEntity* tileEntity);

	// Checks if the area covered by bounds has any tiles already taking the layers specified
	bool checkCanPlace(lost::Bound2D bounds, std::array<bool, 3> layers);
	// Checks if the area covered by bounds has any tiles already taking the layers specified
	bool checkCanPlaceInBounds(lost::Bound2D bounds, std::array<bool, 3> layers);
	// Checks if the area covered by bounds has any tiles already taking the layers specified
	bool checkPlaceOutOfBounds(lost::Bound2D bounds);
	// Checks if the area covered by bounds are all stable
	bool checkStable(lost::Bound2D bounds);

	void setTile(TileRefStruct* tile, int x, int y);
	void updateTileConnections(int x, int y);
	void updateTileNeighbors(int x, int y);

	void addEntity(Entity* entity);

	Tile* getTileAt(int x, int y);
	Tile* forceGetTileAt(int x, int y);

	int loopX(int x);
	float loopX(float x);

	int worldMaxX = 0;
	int worldMinX = 0;

	int worldTileWidth;
	int worldWidth = 20;
private:
	Tile* m_BorderAir = nullptr;

	std::map<int, Chunk*> m_Chunks;
	std::vector<Entity*> m_Entities;

	std::vector<OutOfBoundsTileEntityStruct> m_OutOfBoundsEntities;

	int chunkWidth = 32;
	int chunkHeight = 96;

	Generator* worldGenerator;
};

extern World* g_World;