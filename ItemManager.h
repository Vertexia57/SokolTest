#pragma once
#include "Item.h"
#include "Lost.h"
#include <vector>

// [!] TODO: Create this, this is just the bare bones of what is needed. Figure it out on pen and paper first

class ItemManager
{
public:
	ItemManager();
	~ItemManager();

	void loadItemData(lua_State* loaderState, const char* location);
	void createItemData(lua_State* loaderState);

	inline ItemRefStruct* getItemData(std::string id) { return m_ItemRefStructs[id]; };

private:
	std::map<std::string, ItemRefStruct*> m_ItemRefStructs;
	std::map<std::string, JSONObject*> m_ItemJSONs;
};

extern ItemManager g_ItemManager;