#include "TileManager.h"

TileManager::TileManager()
{
	luaTileData = luaL_newstate();
	luaL_openlibs(luaTileData);
	luaBindDebugPrint(luaTileData);
	checkLua(luaTileData, luaL_dostring(luaTileData, read_text_file("GameData/dataBase.lua").c_str()));
}

TileManager::~TileManager()
{
	for (const auto& [name, object] : tileJSONs)
		delete object;
	tileJSONs.clear();

	for (const auto& [name, object] : connectionDatas)
		delete object;
	connectionDatas.clear();

	for (const auto& [name, object] : tileRefs)
		delete object;
	tileRefs.clear();
}

void TileManager::loadTileData(const char* location)
{
	checkLua(luaTileData, luaL_dostring(luaTileData, read_text_file(location).c_str()));
}

void TileManager::loadTileEntityData(const char* location)
{
	checkLua(luaTileData, luaL_dostring(luaTileData, read_text_file(location).c_str()));
}

void TileManager::createImageData()
{
	checkLua(luaTileData, luaL_dostring(luaTileData, "return data.imageData"));
	JSONObject* imageDatas = LuaStackToJSONObject(luaTileData);

	for (int i = 0; i < imageDatas->getNamesList().size(); i++)
	{
		JSONObject* imageData = imageDatas->getJSONObject(imageDatas->getNamesList()[i]);
		lost::loadImage("GameData/" + imageData->getString("imageFile"), imageDatas->getNamesList()[i]);
	}
}

void TileManager::createTileData()
{
	checkLua(luaTileData, luaL_dostring(luaTileData, "return data.tileData"));
	JSONObject* tileDatas = LuaStackToJSONObject(luaTileData);

	for (int i = 0; i < tileDatas->getNamesList().size(); i++)
	{
		JSONObject* tileData = tileDatas->getJSONObject(tileDatas->getNamesList()[i]);

		tileJSONs[tileData->getString("ID")] = tileData;

		fprintf(stdout, (" [TileManager::createTileData] Loaded Tile: " + tileData->getString("ID") + "\n").c_str());

		// Needs to be ran AFTER images have been loaded from "return data.imageData"
		if (tileData->getObjectList().count("imageData"))
		{
			JSONObject* imageData = tileData->getJSONObject("imageData");
			JSONObject* imageLoadInfo = imageData->getJSONObject("images");
			int loadListSize = imageLoadInfo->getNamesList().size();
			for (int i = 0; i < loadListSize; i++)
				imageLoadInfo->setInt("textureID", lost::getImageID(imageLoadInfo->getString(imageLoadInfo->getNamesList()[i])));
		}

		std::string connectionType = tileData->getString("connectionType");
		if (!connectionDatas.count(connectionType) && connectionType != "none")
			connectionDatas[connectionType] = new ConnectionData(connectionType, read_text_file(("GameData/TileData/ConnectionData/" + connectionType + ".conmeta").c_str()));

		tileRefs[tileData->getString("ID")] = new TileRefStruct(tileData);
	}
}

void TileManager::createTileEntityData()
{
	checkLua(luaTileData, luaL_dostring(luaTileData, "return data.tileEntityData"));
	JSONObject* tileDatas = LuaStackToJSONObject(luaTileData);

	for (int i = 0; i < tileDatas->getNamesList().size(); i++)
	{
		JSONObject* tileData = tileDatas->getJSONObject(tileDatas->getNamesList()[i]);

		tileEntityJSONs[tileData->getString("ID")] = tileData;

		fprintf(stdout, (" [TileManager::createTileEntityData] Loaded TileEntity: " + tileData->getString("ID") + "\n").c_str());

		// Needs to be ran AFTER images have been loaded from "return data.imageData"
		if (tileData->getObjectList().count("imageData"))
		{
			JSONObject* imageData = tileData->getJSONObject("imageData");
			JSONObject* imageLoadInfo = imageData->getJSONObject("images");
			int loadListSize = imageLoadInfo->getNamesList().size();
			for (int i = 0; i < loadListSize; i++)
				imageLoadInfo->setInt("textureID", lost::getImageID(imageLoadInfo->getString(imageLoadInfo->getNamesList()[i])));
		}

		tileEntityRefs[tileData->getString("ID")] = new TileEntityStruct(tileData);
	}
}

void TileManager::finishLoading()
{
	lua_close(luaTileData);
}

JSONObject* TileManager::getTileData(std::string tileName)
{
	return tileJSONs[tileName];
}

JSONObject* TileManager::getTileEntityData(std::string tileName)
{
	return tileEntityJSONs[tileName];
}

TileRefStruct* TileManager::getTileRef(std::string tileName)
{
	return tileRefs[tileName];
}

TileEntityStruct* TileManager::getTileEntityRef(std::string tileName)
{
	return tileEntityRefs[tileName];
}

ConnectionData* TileManager::getConnectionData(std::string connectionName)
{
	return connectionDatas[connectionName];
}

TileManager g_TileManager = TileManager();

ConnectionData::ConnectionData(std::string id_, std::string ConnectionMetadata)
{
	// Sets the name for the connection data
	id = id_;

	// Splits the connection data to each line
	std::vector<std::string> lines = split(ConnectionMetadata, '\n');

	std::vector<std::string> connections;
	
	// Splits up the read file into connection data slots
	std::string currentProcess = "";
	for (std::string s : lines)
	{
		if (s.at(0) == '-' && currentProcess.size() > 0)
		{
			connections.push_back(currentProcess);
			currentProcess = "";
		}
		else if (currentProcess.size() > 0)
		{
			currentProcess += '\n';
		}

		currentProcess += s;
	}

	connections.push_back(currentProcess);

	// Loops through the connection data
	for (std::string s : connections)
	{
		// Splits it up into lines again
		std::vector<std::string> data = split(s, '\n');

		// Finds the id and type at the end of each "--- tx" where t is type and x is the value
		std::string typeAndValue = split(data.at(0), ' ').at(1);

		int id = 0;
		bool rotates = false;
		bool halfFlip = false;

		// Checks if the type is the base type of no rotations
		if (isdigit(typeAndValue.at(0)))
		{
			id = std::stoi(typeAndValue);
		}
		else
		{
			// A code was found
			id = std::stoi(typeAndValue.substr(1, typeAndValue.size() - 1));
			if (typeAndValue.at(0) == 'r')
			{
				rotates = true;
			}
			else
			{
				rotates = true;
				halfFlip = true;
			}
		}

		// Reads the matrix from the strings and puts them in the array
		std::array<int, 9> conMatrix{ 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		for (int y = 0; y < 3; y++)
		{
			for (int x = 0; x < 3; x++)
			{
				char val = tolower(data.at(y + 1).at(x));
				if (val == 'x')
					conMatrix[x + y * 3] = 0;
				else if (val == '/')
					conMatrix[x + y * 3] = 1;
				else if (val == '#')
					conMatrix[x + y * 3] = 2;
			}
		}
		// Adds the unchanged matrix to the matrix list
		connectionMatricies.push_back(ConnectionMatrix{ conMatrix, id });

		if (rotates)
		{
			// Do half flip rotations, where there's only one rotation
			int xOut = 0;
			int yOut = 0;

			std::array<int, 9> rotatedMatrix{ 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			for (int x = 0; x < 3; x++)
			{
				xOut = 0;
				for (int y = 2; y >= 0; y--)
				{
					rotatedMatrix[xOut + yOut * 3] = conMatrix[x + y * 3];
					xOut++;
				}
				yOut++;
			}
			connectionMatricies.push_back(ConnectionMatrix{ rotatedMatrix, id + 1 });

			if (!halfFlip)
			{
				// Do full rotations, where theres 4
				xOut = 0;
				yOut = 0;

				std::array<int, 9> rotatedMatrix{ 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				for (int y = 2; y >= 0; y--)
				{
					xOut = 0;
					for (int x = 2; x >= 0; x--)
					{
						rotatedMatrix[xOut + yOut * 3] = conMatrix[x + y * 3];
						xOut++;
					}
					yOut++;
				}
				connectionMatricies.push_back(ConnectionMatrix{ rotatedMatrix, id + 2 });

				xOut = 0;
				yOut = 0;

				rotatedMatrix = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				for (int x = 2; x >= 0; x--)
				{
					xOut = 0;
					for (int y = 0; y < 3; y++)
					{
						rotatedMatrix[xOut + yOut * 3] = conMatrix[x + y * 3];
						xOut++;
					}
					yOut++;
				}
				connectionMatricies.push_back(ConnectionMatrix{ rotatedMatrix, id + 3 });
			}
		}
	}
}

ConnectionData::~ConnectionData()
{
}