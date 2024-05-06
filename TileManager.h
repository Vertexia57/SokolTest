#pragma once
#include "JSON.h"
#include "Lost.h"
#include "Tile.h"
#include <array>

struct ConnectionMatrix
{
	std::array<int, 9> connectionMatrix;
	/*
	 * 0 - Represents An X, which means it ignores this tile.
	 * 1 - Represents a  /, which means it checks for a non-connecting tile.
	 * 2 - Represents a  #, which means it checks for a connecting tile.
	 */
	int id = 0;
};

class ConnectionData
{
private:
	std::string id;

	std::vector<ConnectionMatrix> connectionMatricies;
public:
	ConnectionData(std::string id_, std::string ConnectionMetadata);
	~ConnectionData();

	inline std::vector<ConnectionMatrix>* getMatricies() { return &connectionMatricies; };
};

class TileManager
{
public:
	TileManager();
	~TileManager();

	void loadTileData(lua_State* loaderState, const char* location);
	void loadTileEntityData(lua_State* loaderState, const char* location);
	
	void createImageData(lua_State* loaderState);
	void createTileData(lua_State* loaderState);
	void createTileEntityData(lua_State* loaderState);

	JSONObject* getTileData(std::string tileName);
	JSONObject* getTileEntityData(std::string tileName);

	TileRefStruct* getTileRef(std::string tileName);
	TileEntityStruct* getTileEntityRef(std::string tileName);

	ConnectionData* getConnectionData(std::string connectionName);

	std::vector<TileEntityStruct*> buildingRefs;
private:
	std::map<std::string, JSONObject*> tileJSONs;
	std::map<std::string, TileRefStruct*> tileRefs;
	std::map<std::string, ConnectionData*> connectionDatas;

	std::map<std::string, JSONObject*> tileEntityJSONs;
	std::map<std::string, TileEntityStruct*> tileEntityRefs;
};

extern TileManager g_TileManager;