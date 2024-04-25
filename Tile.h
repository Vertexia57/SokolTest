#pragma once
#include "Lost.h"
#include "TileEntity.h"
#include "JSON.h"
#include <set>
#include <array>

enum
{
	TILE_LAYER_FOREGROUND,
	TILE_LAYER_BACKGROUND,
	TILE_LAYER_DETAIL
};

struct TileRefStruct
{
	bool empty = false;

	std::string name = "Null";
	std::string ID = "Null";
	std::string description = "Null";

	TextureID texture = 0;
	int totalFrames = 1;
	int totalVariants = 1;

	std::string connectionType = "none";
	std::string connectionGroup = "none";
	std::set<std::string> connectsTo = {};

	std::array<bool, 3> fillsLayers = { false, false, false };

	TileRefStruct(JSONObject* tileData)
	{
		name = tileData->getString("Name");
		ID = tileData->getString("ID");
		description = tileData->getString("Description");

		empty = tileData->getBool("Empty");

		if (tileData->getObjectList().count("ImageData"))
		{
			JSONObject* imageData = tileData->getJSONObject("ImageData");

			texture = imageData->getJSONArray("Images")->getJSONObject(0)->getInt("TextureID");
			totalFrames = imageData->getInt("Frames");
			totalVariants = imageData->getInt("Variants");
		}

		connectionType = tileData->getString("ConnectionType");
		if (connectionType != "none")
		{
			JSONObject* connectionData = tileData->getJSONObject("ConnectionData");
			connectionGroup = connectionData->getString("ConnectionGroup");
			JSONArray* connectsToArray = connectionData->getJSONArray("ConnectsTo");
			for (int i = 0; i < connectsToArray->size(); i++)
				connectsTo.insert(connectsToArray->getString(i));
		}

		JSONArray* fillsLayersArray = tileData->getJSONArray("FillsLayers");
		for (int i = 0; i < fillsLayersArray->size(); i++)
			fillsLayers[i] = fillsLayersArray->getBool(i);
	}
};

class Tile
{
public:
	Tile(lost::IntVector2D position, TileRefStruct* referenceStruct_);
	~Tile();

	void addTileEntityRef(TileEntity* ref);
	void removeTileEntityRef(TileEntity* ref);

	void setTextureVariant(uint32_t textureVariant);

	void render();

	bool filledLayers[3] = { false, false, false };

	lost::IntVector2D pos;
	std::vector<TileEntity*> tileEntitiesWithin;

	bool empty = false;

	TileRefStruct* referenceStruct;
private:
	TextureID m_TileID = 0;
};