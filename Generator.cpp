#include "Generator.h"
#include "Chunk.h"
#include <filesystem>

Generator::Generator(std::string generatorLocation_)
{
	generatorCode = read_text_file(generatorLocation_.c_str());
	generatorLocation = generatorLocation_;

	std::vector<std::string> Paths = split(generatorLocation, '\\');
	std::string startString = "";
	for (int i = 0; i < Paths.size() - 1; i++)
	{
		if (i == Paths.size() - 2)
			startString += Paths.at(i);
		else
			startString += Paths.at(i) + "\\";
	}

	std::string intermediateString = (std::filesystem::current_path().string() + "\\" + startString);
	std::string finalString = "";
	for (char c : intermediateString)
	{
		if (c != '\\')
			finalString += c;
		else
			finalString += "\\\\";
	}

	generatorPathStr = finalString;

}

Generator::~Generator()
{
}

ChunkDataStruct Generator::generateChunk(int chunkX, int width, int height, int worldWidth)
{
	lua_State* L;
	L = luaL_newstate();
	luaL_openlibs(L);
	luaBindDebugPrint(L);

	checkLua(L, luaL_dostring(L, (
		"chunkX = " + std::to_string(chunkX) + "\n" +
		"chunkWidth = " + std::to_string(width) + "\n" +
		"chunkHeight = " + std::to_string(height) + "\n" +
		"generatorSeed = " + std::to_string(seed) + "\n" +
		"worldWidth = " + std::to_string(worldWidth) + "\n" +
		"localDir = \"" + generatorPathStr + "\"").c_str()
	));

	checkLua(L, luaL_dostring(L, (generatorCode).c_str()));

	std::vector<int> MapRefIDs = {};
	MapRefIDs.reserve(width * height);
	std::vector<std::string> MapTileAtlas = {};

	// Reads Fourth given return value
	lua_pushnil(L);
	while (lua_next(L, -2)) {
		MapTileAtlas.push_back(lua_tostring(L, -1));
		lua_pop(L, 1);
	}
	lua_pop(L, 1); // Clears it from the lua stack

	// Reads Third given return value
	lua_pushnil(L);
	while (lua_next(L, -2)) {
		lua_pushnil(L); // Checks next sub array of the value
		while (lua_next(L, -2)) {
			MapRefIDs.push_back((int)lua_tonumber(L, -1));
			lua_pop(L, 1);
		}
		lua_pop(L, 1); // Clears it from the lua stack
	}
	lua_pop(L, 1); // Clears it from the lua stack

	ChunkDataStruct dataStruct = {};
	dataStruct.width = width;
	dataStruct.height = height;
	dataStruct.tileMap.reserve(width * height);

	std::vector<TileRefStruct*> refList;
	refList.reserve(width * height);
	for (int i = 0; i < MapRefIDs.size(); i++)
		refList.push_back(g_TileManager.getTileRef(MapTileAtlas[MapRefIDs[i]]));

	dataStruct.tileMap = refList;

	std::vector<TileEntityCreateStruct> MapEntityCreates = {};
	std::vector<std::string> MapTileEntityAtlas = {};

	// Reads Second given return value
	lua_pushnil(L);
	while (lua_next(L, -2)) {
		MapTileEntityAtlas.push_back(lua_tostring(L, -1));
		lua_pop(L, 1);
	}
	lua_pop(L, 1); // Clears it from the lua stack

	// Reads First given return value
	lua_pushnil(L);
	while (lua_next(L, -2)) {
		lua_pushnil(L); // Checks next sub array of the value

		TileEntityCreateStruct createStruct = {};

		lua_next(L, -2);
		createStruct.tileEntityRef = g_TileManager.getTileEntityRef(MapTileEntityAtlas[(int)lua_tonumber(L, -1)]);
		lua_pop(L, 1);

		lua_next(L, -2);
		createStruct.position.x = (float)lua_tonumber(L, -1);
		lua_pop(L, 1);

		lua_next(L, -2);
		createStruct.position.y = (float)lua_tonumber(L, -1);
		lua_pop(L, 1);

		MapEntityCreates.push_back(createStruct);

		lua_pop(L, 2); // Clears it from the lua stack
	}
	lua_pop(L, 1); // Clears it from the lua stack

	dataStruct.tileEntities = MapEntityCreates;

	lua_close(L);

	return dataStruct;
}
