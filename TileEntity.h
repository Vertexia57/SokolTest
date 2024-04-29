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
	bool randomVariant = false;

	float width = 0.0f;
	float height = 0.0f;
	float placementOffsetX = 0.0f;
	float placementOffsetY = 0.0f;

	std::array<bool, 3> fillsLayers = { false, false, false };

	TileEntityStruct(JSONObject* tileEntityData)
	{
		name = tileEntityData->getString("name");
		ID = tileEntityData->getString("ID");
		description = tileEntityData->getString("description");

		if (tileEntityData->getObjectList().count("imageData"))
		{
			JSONObject* imageData = tileEntityData->getJSONObject("imageData");

			texture = imageData->getJSONObject("images")->getInt("textureID");
			totalFrames = imageData->getInt("frames");
			totalVariants = imageData->getInt("variants");
			if (imageData->getObjectList().count("randomVariant"))
				randomVariant = imageData->getBool("randomVariant");
		}

		width = tileEntityData->getFloat("width");
		height = tileEntityData->getFloat("height");
		placementOffsetX = tileEntityData->getFloat("placementOffsetX");
		placementOffsetY = tileEntityData->getFloat("placementOffsetY");

		stableGround = tileEntityData->getBool("stable");
		collidable = tileEntityData->getBool("collidable");
		requiresSupport = tileEntityData->getBool("requiresSupport");

		JSONObject* fillsLayersArray = tileEntityData->getJSONObject("fillsLayers");
		for (int i = 0; i < fillsLayersArray->getNamesList().size(); i++)
			fillsLayers[i] = fillsLayersArray->getBool(fillsLayersArray->getNamesList()[i]);
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
	TextureID m_Variant;
};

