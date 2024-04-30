#pragma once
#include "Item.h"

class Container
{
public:
	Container(int slots);
	~Container();

	// Returns the amount of that item that COULDN'T be added to the container
	int addItem(ItemRefStruct* refStruct, int count);
	// Returns the amount of that item that COULDN'T be added to the container
	int addItem(Item item);

	// Returns the amount of that item that COULDN'T be added to the container
	int addItemToSlot(Item& item, int slot);

	bool isSlotTaken(int slot);
	void setItem(Item item, int slot);

	inline Item* getItem(int slot) { return &m_Items[slot]; };

	int findAccessableSlot(Item& item);

	int size = 0;
private:
	std::vector<Item> m_Items;
};

