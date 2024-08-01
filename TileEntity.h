#pragma once
#include "JSON.h"
#include "Lost.h"
#include "Container.h"
#include <array>
#include <functional>
#include "ItemManager.h"

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
	std::vector<IdCountPair> cost;
	float powerIdleUsage = 0.0f;
	float powerActiveUsage = 0.0f;
	float powerProduce = 0.0f;
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

			JSONObject* costs = buildingData->getJSONObject("cost");
			for (int i = 0; costs->getObjectList().count(std::to_string(i)); i++)
			{
				JSONObject* pair = costs->getJSONObject(std::to_string(i));
				cost.push_back({ pair->getString("item"), pair->getInt("count") });
			}

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

		if (tileEntityData->getObjectList().count("powerData"))
		{
			JSONObject* powerData = tileEntityData->getJSONObject("powerData");
			powerIdleUsage = powerData->getFloat("powerIdleUsage");
			powerActiveUsage = powerData->getFloat("powerActiveUsage");
			powerProduce = powerData->getFloat("powerProduce");
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
	
	inline bool hasInventory() const { return (m_Storage != nullptr) || m_HasInventory; };
	inline Container* getInventory() const { return m_Storage; };

	inline bool beenTileUpdated() const { return m_TileUpdated; };
	inline float getPowerConsumption() const { return m_Active ? m_ActiveConsumption * m_ConsumptionMult : m_IdleConsumption * m_ConsumptionMult; };
	inline float getPowerProduce() const { return m_BaseProduce * m_ProduceMult; };
	inline uint32_t getPowerCircuit() const { return m_PowerCircuit; };
	inline void setPowerCircuit(uint32_t id) { m_PowerCircuit = id; };

	virtual void init();

	virtual void update();
	virtual void tileUpdate();
	virtual void checkPowerCircuit();

	virtual void render();
	virtual void renderAt(lost::Vector2D pos);
	virtual void renderForeground();
	virtual void renderForegroundAt(lost::Vector2D pos);
	virtual void renderHitbox();

	virtual void removeItem(Item& item, int count, bool output = true);
	virtual Item extractItem(int count, bool output = true);
	virtual void insertItem(Item& item);
	virtual bool canInsert(Item& item) const;

	bool fillsLayers[3] = { false, false, false };

	lost::Vector2D position;

	bool updates = false;
	bool collidable = true;
	bool stable = false;

	bool interactable = false;
	virtual void mouseInteractFunction();

	TileEntityStruct* tileEntityRef;
	lost::Vector2D relativeVelocity = { 0.0f, 0.0f }; // Used for conveyer belts and pipes

	std::string tileType = "base";
protected:
	// Used for ducts and inventories
	Container* m_Storage = nullptr;
	bool m_HasInventory = false;

	// Used for buildings that consume power
	uint32_t m_PowerCircuit = 0xffffffff; // 32 bit integer limit = no circuit
	float m_ConsumptionMult = 1.0f;
	float m_ActiveConsumption = 0.0f;
	float m_IdleConsumption = 0.0f;
	bool m_Active = true;
	float m_ProduceMult = 1.0f;
	float m_BaseProduce = 0.0f;

	lost::Bound2D m_Hitbox;
	TextureID m_Variant;
	uint32_t m_Rotation = 0;

	uint32_t m_Frame = 0;
	double m_TimeFromLastFrame = 0.0;

	bool m_TileUpdated = false;
};

