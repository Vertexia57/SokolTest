#pragma once
#include "lost.h"
#include <map>
#include "Tile.h"
#include "TileManager.h"
#include "Random.h"

struct ChunkDataStruct
{
	int width;
	int height;
	std::vector<TileRefStruct*> tileMap;
	// [!] TODO: Add support for tileEntities
};

class Generator
{
public:
	Generator(std::string generatorLocation_);
	~Generator();

	ChunkDataStruct generateChunk(int chunkX, int width, int height);

	long seed = random(LONG_MIN, LONG_MAX);

private:
	lua_State* L;

	std::string generatorLocation;
	std::string generatorPathStr;
	std::string generatorCode;
};

