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
	bool stableGround = false;
	bool collidable = false;

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

	JSONObject* extraData = nullptr;
	JSONObject* extraLocalData = nullptr;

	TileRefStruct(JSONObject* tileData)
	{
		name = tileData->getString("name");
		ID = tileData->getString("ID");
		description = tileData->getString("description");

		empty = tileData->getBool("empty");
		stableGround = tileData->getBool("stable");
		collidable = tileData->getBool("collidable");

		if (tileData->getObjectList().count("imageData"))
		{
			JSONObject* imageData = tileData->getJSONObject("imageData");

			texture = imageData->getJSONObject("images")->getInt("textureID");
			totalFrames = imageData->getInt("frames");
			totalVariants = imageData->getInt("variants");
		}

		connectionType = tileData->getString("connectionType");
		if (connectionType != "none")
		{
			JSONObject* connectionData = tileData->getJSONObject("connectionData");
			connectionGroup = connectionData->getString("connectionGroup");
			JSONObject* connectsToArray = connectionData->getJSONObject("connectsTo");
			for (int i = 0; i < connectsToArray->getNamesList().size(); i++)
				connectsTo.insert(connectsToArray->getString(connectsToArray->getNamesList()[i]));
		}

		JSONObject* fillsLayersArray = tileData->getJSONObject("fillsLayers");
		for (int i = 0; i < fillsLayersArray->getNamesList().size(); i++)
			fillsLayers[i] = fillsLayersArray->getBool(fillsLayersArray->getNamesList()[i]);

		if (tileData->getObjectList().count("extraData") > 0)
			extraData = tileData->getJSONObject("extraData");
		if (tileData->getObjectList().count("extraLocalData") > 0)
			extraLocalData = tileData->getJSONObject("extraLocalData");
	}
};

class Tile
{
public:
	Tile(lost::IntVector2D position, TileRefStruct* referenceStruct_);
	virtual ~Tile();

	void addTileEntityRef(TileEntity* ref);
	void removeTileEntityRef(TileEntity* ref);

	void setTextureVariant(uint32_t textureVariant);

	virtual void tileUpdate();
	virtual void forceTileUpdate();

	void render();
	void renderAt(lost::Vector2D position);

	void setPowerCircuit(uint32_t id);
	void joinPowerCircuit(uint32_t id);
	void leavePowerCircuit();
	inline uint32_t getPowerCircuit() const { return m_PowerCircuit; };

	bool filledLayers[3] = { false, false, false };

	lost::IntVector2D pos;
	std::vector<TileEntity*> tileEntitiesWithin;

	bool empty = false;

	TileRefStruct* referenceStruct;
	JSONObject* extraLocalData = nullptr;
private:
	TextureID m_TileID = 0;
	uint32_t m_PowerCircuit = 0xffffffff;
	uint32_t m_ConnectedPowerSources = 0;
};