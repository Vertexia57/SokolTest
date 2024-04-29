#pragma once
#include "Item.h"
#include <vector>

// [!] TODO: Create this, this is just the bare bones of what is needed. Figure it out on pen and paper first

class ItemManager
{
public:
	ItemManager();
	~ItemManager();

	void loadItemData(const char* location);
	void createItemData();

private:
	std::vector<ItemRefStruct*> m_ItemRefStructs;
};

extern ItemManager g_ItemManager;