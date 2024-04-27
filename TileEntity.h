#pragma once
#include "JSON.h"
#include "Lost.h"
#include <array>

struct TileEntityStruct
{
	bool stableGround = false;
	bool collidable = false;
	bool requiresSupport = false;

	std::string name = "Null";
	std::string ID = "Null";
	std::string description = "Null";

	TextureID texture = 0;
	int totalFrames = 1;
	int totalVariants = 1;

	float width = 0.0f;
	float height = 0.0f;
	float placementOffsetX = 0.0f;
	float placementOffsetY = 0.0f;

	std::array<bool, 3> fillsLayers = { false, false, false };

	TileEntityStruct(JSONObject* tileEntityData)
	{
		name = tileEntityData->getString("Name");
		ID = tileEntityData->getString("ID");
		description = tileEntityData->getString("Description");

		if (tileEntityData->getObjectList().count("ImageData"))
		{
			JSONObject* imageData = tileEntityData->getJSONObject("ImageData");

			texture = imageData->getJSONArray("Images")->getJSONObject(0)->getInt("TextureID");
			totalFrames = imageData->getInt("Frames");
			totalVariants = imageData->getInt("Variants");
		}

		width = tileEntityData->getFloat("Width");
		height = tileEntityData->getFloat("Height");
		placementOffsetX = tileEntityData->getFloat("PlacementOffsetX");
		placementOffsetY = tileEntityData->getFloat("PlacementOffsetY");

		stableGround = tileEntityData->getBool("Stable");
		collidable = tileEntityData->getBool("Collidable");
		requiresSupport = tileEntityData->getBool("RequiresSupport");

		JSONArray* fillsLayersArray = tileEntityData->getJSONArray("FillsLayers");
		for (int i = 0; i < fillsLayersArray->size(); i++)
			fillsLayers[i] = fillsLayersArray->getBool(i);
	}

};

class TileEntity
{
public:
	TileEntity(TileEntityStruct* tileEntityRef_);
	~TileEntity();

	// Sets the hitbox for the tile, with a max of 64 x 64
	void setHitbox(lost::Bound2D hitbox);
	void setPosition(lost::Vector2D position_);
	lost::Bound2D getHitbox();

	void render();
	void renderAt(lost::Vector2D pos);
	void renderHitbox();

	bool fillsLayers[3] = { false, false, false };

	lost::Vector2D position;

	bool collidable = true;

	TileEntityStruct* tileEntityRef;
private:
	lost::Bound2D m_Hitbox;
	TextureID m_TileID;
};

