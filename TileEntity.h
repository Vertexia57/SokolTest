#pragma once
#include "JSON.h"
#include "Lost.h"
#include <array>
#include <functional>

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
	int totalRotations = 1;
	bool randomVariant = false;
	std::vector<int> rotationVariants = {};

	bool animated = false;
	float frameRate = 1.0f;

	float width = 0.0f;
	float height = 0.0f;
	uint32_t renderLayer = 0;
	float placementOffsetX = 0.0f;
	float placementOffsetY = 0.0f;

	std::array<bool, 3> fillsLayers = { false, false, false };

	bool building = false;
	float cost = 0.0f;
	float powerUsage = 0.0f;
	std::string updateAction = "none";
	JSONObject* updateData = nullptr;

	bool container = false;
	std::vector<std::pair<std::string, int>> slotTypes;
	int slotCount = 0;

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

			animated = imageData->getBool("animated");
			if (animated)
			{
				JSONObject* animationData = imageData->getJSONObject("animationData");
				frameRate = animationData->getFloat("frameRate");
			}
		}

		width = tileEntityData->getFloat("width");
		height = tileEntityData->getFloat("height");
		renderLayer = tileEntityData->getInt("renderLayer");
		placementOffsetX = tileEntityData->getFloat("placementOffsetX");
		placementOffsetY = tileEntityData->getFloat("placementOffsetY");

		stableGround = tileEntityData->getBool("stable");
		collidable = tileEntityData->getBool("collidable");
		requiresSupport = tileEntityData->getBool("requiresSupport");

		JSONObject* fillsLayersArray = tileEntityData->getJSONObject("fillsLayers");
		for (int i = 0; i < fillsLayersArray->getNamesList().size(); i++)
			fillsLayers[i] = fillsLayersArray->getBool(fillsLayersArray->getNamesList()[i]);
		
		building = tileEntityData->getBool("building");
		if (building)
		{
			JSONObject* buildingData = tileEntityData->getJSONObject("buildingData");

			powerUsage = buildingData->getFloat("powerUsage");
			cost = buildingData->getFloat("cost");
			updateAction = buildingData->getString("updateAction");
			updateData = buildingData->getJSONObject("updateData");
			totalRotations = buildingData->getInt("rotationStates");
			if (buildingData->getObjectList().count("rotationVariants") == 1)
			{
				JSONObject* rotationVariantList = buildingData->getJSONObject("rotationVariants");
				for (int i = 0; rotationVariantList->getObjectList().count(std::to_string(i)) > 0; i++)
				{
					rotationVariants.push_back(rotationVariantList->getInt(std::to_string(i)));
				}
			}
			else
			{
				for (int i = 0; i < totalRotations; i++)
					rotationVariants.push_back(i);
			}
		}

		container = tileEntityData->getObjectList().count("containerData") == 1;
		if (container)
		{
			JSONObject* slotData = tileEntityData->getJSONObject("containerData")->getJSONObject("slots");
			for (int i = 0; slotData->getObjectList().count(std::to_string(i)) > 0; i++)
			{
				std::pair<std::string, int> slotPair;
				JSONObject* slotPairData = slotData->getJSONObject(std::to_string(i));
				slotPair = { slotPairData->getString("0"), slotPairData->getInt("1") };
				slotTypes.push_back(slotPair);
				slotCount += slotPair.second;
			}
		}
	}

};

class TileEntity
{
public:
	TileEntity(TileEntityStruct* tileEntityRef_, uint32_t rotation = 0);
	virtual ~TileEntity();

	void setHitbox(lost::Bound2D hitbox);
	void setPosition(lost::Vector2D position_);
	lost::Bound2D getHitbox();

	virtual void init();

	virtual void update();

	virtual void render();
	virtual void renderAt(lost::Vector2D pos);
	virtual void renderForeground();
	virtual void renderForegroundAt(lost::Vector2D pos);
	virtual void renderHitbox();

	bool fillsLayers[3] = { false, false, false };

	lost::Vector2D position;

	bool updates = false;
	bool collidable = true;

	bool interactable = false;
	virtual void mouseInteractFunction();

	TileEntityStruct* tileEntityRef;
	lost::Vector2D relativeVelocity = { 0.0f, 0.0f }; // Used for conveyer belts and pipes

	std::string tileType = "base";
protected:
	lost::Bound2D m_Hitbox;
	TextureID m_Variant;
	uint32_t m_Rotation = 0;

	uint32_t m_Frame = 0;
	double m_TimeFromLastFrame = 0.0;
};

