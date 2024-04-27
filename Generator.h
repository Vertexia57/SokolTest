#pragma once
#include "lost.h"
#include <map>
#include "Tile.h"
#include "TileManager.h"
#include "Random.h"

struct TileEntityCreateStruct
{
	TileEntityStruct* tileEntityRef;
	lost::Vector2D position;
};

struct ChunkDataStruct
{
	int width;
	int height;
	std::vector<TileRefStruct*> tileMap;
	std::vector<TileEntityCreateStruct> tileEntities;
};

class Generator
{
public:
	Generator(std::string generatorLocation_);
	~Generator();

	ChunkDataStruct generateChunk(int chunkX, int width, int height, int worldWidth);

	long seed = random(LONG_MIN, LONG_MAX);

private:
	lua_State* L;

	std::string generatorLocation;
	std::string generatorPathStr;
	std::string generatorCode;
};

