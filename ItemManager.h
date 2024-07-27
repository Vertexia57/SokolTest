#pragma once
#include "Item.h"
#include "Lost.h"
#include <vector>
#include <unordered_map>

// [!] TODO: Create this, this is just the bare bones of what is needed. Figure it out on pen and paper first

struct RecipieRefStruct
{
	RecipieRefStruct(JSONObject* ref)
	{
		craftingGroup = ref->getString("craftingGroup");

		JSONObject* inputs = ref->getJSONObject("ingredients");
		for (int i = 0; inputs->getObjectList().count(std::to_string(i)); i++)
		{
			JSONObject* pair = inputs->getJSONObject(std::to_string(i));
			ingredients.push_back({ pair->getString("item"), pair->getInt("count") });
		}

		JSONObject* outputs = ref->getJSONObject("results");
		for (int i = 0; outputs->getObjectList().count(std::to_string(i)); i++)
		{
			JSONObject* pair = outputs->getJSONObject(std::to_string(i));
			results.push_back({ pair->getString("item"), pair->getInt("count") });
		}

		if (ref->getObjectList().count("icon"))
			icon = ref->getInt("icon");
		else
			icon = (TextureID)(-1);

		timeToCraft = ref->getFloat("timeToCraft");
	}

	struct IdCountPair 
	{
		std::string id;
		int count;
	};

	std::string craftingGroup = "base";
	TextureID icon;

	float timeToCraft = 1.0f;

	std::vector<IdCountPair> ingredients;
	std::vector<IdCountPair> results;
};

class ItemManager
{
public:
	ItemManager();
	~ItemManager();

	void loadItemData(lua_State* loaderState, const char* location);
	void createItemData(lua_State* loaderState);

	inline ItemRefStruct* getItemData(std::string id) { return m_ItemRefStructs[id]; };

	inline RecipieRefStruct* getRecipieData(std::string id) { return m_RecipieRefStructs[id]; };

	inline const std::unordered_map<std::string, RecipieRefStruct*>& getCraftingGroupData(std::string id) { return m_RecipieCraftingGroups[id]; };
private:
	std::unordered_map<std::string, ItemRefStruct*> m_ItemRefStructs;
	std::unordered_map<std::string, JSONObject*> m_ItemJSONs;

	std::unordered_map<std::string, std::unordered_map<std::string, RecipieRefStruct*>> m_RecipieCraftingGroups;
	std::unordered_map<std::string, RecipieRefStruct*> m_RecipieRefStructs;
	std::map<std::string, JSONObject*> m_RecipieJSONs;
};

extern ItemManager g_ItemManager;