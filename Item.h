#pragma once
#include "JSON.h"
#include "Lost.h"

struct ItemRefStruct
{
	TextureID textureID = 0;
	int variants = 1;
	int frames = 1;
	bool randomVariant = false;

	int maxStack = 1;
	bool placesTile = false;
	std::string placesTileID = "";
	bool placesTileEntity = false;
	std::string placesTileEntityID = "";

	std::string name = "";
	std::string itemID = "";

	ItemRefStruct(JSONObject* itemData)
	{
		std::map<std::string, int> objectList = itemData->getObjectList();
		maxStack = itemData->getInt("stackCount");
		placesTile = (objectList.count("placesTile") > 0);
		if (placesTile)
			placesTileID = itemData->getString("placesTile");
		placesTileEntity = (objectList.count("placesTileEntity") > 0);
		if (placesTileEntity)
			placesTileEntityID = itemData->getString("placesTileEntity");
		
		name = itemData->getString("name");
		itemID = itemData->getString("ID");

		JSONObject* imageData = itemData->getJSONObject("imageData");
		textureID = imageData->getInt("textureID");
		variants = imageData->getInt("variants");
		frames = imageData->getInt("frames");
		randomVariant = imageData->getBool("randomVariant");
	}

};

struct Item
{
	Item();
	Item(ItemRefStruct* refStruct_);
	~Item();

	bool empty = false;
	bool locked = false;
	bool output = false;
	ItemRefStruct* refStruct = nullptr;

	std::string itemName = "";
	std::string itemID = "";
	int StackSize = 0;
	int maxStackSize = 0;

	TextureID textureID = 0;
	int variant = 0;
};

