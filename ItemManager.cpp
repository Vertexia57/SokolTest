#include "ItemManager.h"

ItemManager::ItemManager()
{
}

ItemManager::~ItemManager()
{
	for (const auto& [name, object] : m_ItemRefStructs)
		delete object;
}

void ItemManager::loadItemData(lua_State* loaderState, const char* location)
{
	checkLua(loaderState, luaL_dostring(loaderState, read_text_file(location).c_str()));
}

void ItemManager::createItemData(lua_State* loaderState)
{
	checkLua(loaderState, luaL_dostring(loaderState, "return data.itemData"));
	JSONObject* itemDatas = LuaStackToJSONObject(loaderState);

	for (int i = 0; i < itemDatas->getNamesList().size(); i++)
	{
		JSONObject* itemData = itemDatas->getJSONObject(itemDatas->getNamesList()[i]);

		m_ItemJSONs[itemData->getString("ID")] = itemData;

		fprintf(stdout, (" [ItemManager::createItemData] Loaded Item: " + itemData->getString("ID") + "\n").c_str());

		// Needs to be ran AFTER images have been loaded from "return data.imageData"
		if (itemData->getObjectList().count("imageData"))
		{
			JSONObject* imageData = itemData->getJSONObject("imageData");
			imageData->setInt("textureID", lost::getImageID(imageData->getString("imageID")));
		}

		m_ItemRefStructs[itemData->getString("ID")] = new ItemRefStruct(itemData);
	}
}

ItemManager g_ItemManager = ItemManager();