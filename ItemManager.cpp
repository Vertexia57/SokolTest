#include "ItemManager.h"

ItemManager::ItemManager()
{
}

ItemManager::~ItemManager()
{
	for (const auto& [name, object] : m_ItemRefStructs)
		delete object;
	for (const auto& [name, object] : m_RecipieRefStructs)
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

	checkLua(loaderState, luaL_dostring(loaderState, "return data.itemRecipies"));
	JSONObject* recipieDatas = LuaStackToJSONObject(loaderState);

	for (int i = 0; i < recipieDatas->getNamesList().size(); i++)
	{
		JSONObject* recipieData = recipieDatas->getJSONObject(recipieDatas->getNamesList()[i]);

		m_RecipieJSONs[recipieData->getString("ID")] = recipieData;

		fprintf(stdout, (" [ItemManager::createRecipieData] Loaded Recipie: " + recipieData->getString("ID") + "\n").c_str());

		RecipieRefStruct* recipieRef = new RecipieRefStruct(recipieData);
		m_RecipieRefStructs[recipieData->getString("ID")] = recipieRef;
		m_RecipieCraftingGroups[recipieData->getString("craftingGroup")][recipieData->getString("ID")] = recipieRef;
	}
}

ItemManager g_ItemManager = ItemManager();