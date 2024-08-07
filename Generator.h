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

struct GenerateStructureStruct
{
	std::string structureID;
	lost::IntVector2D startPos;
	lost::Bound2D bounds = { 0.0f, 0.0f, 0.0f, 0.0f };
	JSONObject* extraData = nullptr;
};

struct ChunkDataStruct
{
	int width;
	int height;
	std::vector<TileRefStruct*> tileMap;
	std::vector<TileEntityCreateStruct> tileEntities;
	std::vector<GenerateStructureStruct> structures;
};

class Chunk;

class Generator
{
public:
	Generator(std::string generatorLocation_);
	~Generator();

	ChunkDataStruct generateChunk(int chunkX, int width, int height, int worldWidth);

	long seed = random(LONG_MIN, LONG_MAX);

private:
	std::string generatorLocation;
	std::string generatorPathStr;
	std::string generatorCode;
};

