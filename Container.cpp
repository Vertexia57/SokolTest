#include "Container.h"

Container::Container(int slots)
{
	m_Items.resize(slots);
	size = slots;
}

Container::~Container()
{
	m_Items.clear();
}

int Container::addItem(ItemRefStruct* refStruct, int count)
{
	Item item = Item(refStruct);
	item.StackSize = count;
	return addItem(item);
}

int Container::addItem(Item item)
{
	int slot = findAccessableSlot(item);
	int count = item.StackSize;

	if (slot != -1)
	{
		// Loop until the item has been fully added to the container
		while (count > 0)
		{

			if (isSlotTaken(slot))
			{
				// Check the amount to fill the current item slot
				int amountToFill = (m_Items[slot].StackSize + count > m_Items[slot].refStruct->maxStack) ? m_Items[slot].refStruct->maxStack - m_Items[slot].StackSize : count;
				m_Items[slot].StackSize += amountToFill;
				count -= amountToFill;
			}
			else
			{
				m_Items[slot] = item;
				count = 0;
			}

			slot = findAccessableSlot(item);

			if (slot == -1)
				break;
		}
	}

	return count;
}

int Container::addItemToSlot(Item& item, int slot)
{
	int count = item.StackSize;

	if (isSlotTaken(slot))
	{

		int amountToFill = (m_Items[slot].StackSize + count > m_Items[slot].refStruct->maxStack) ? m_Items[slot].refStruct->maxStack - m_Items[slot].StackSize : count;
		m_Items[slot].StackSize += amountToFill;
		count -= amountToFill;
	}
	else
	{
		m_Items[slot] = item;
		count = 0;
	}

	return count;
}

bool Container::isSlotTaken(int slot)
{
	return !m_Items[slot].empty;
}

void Container::setItem(Item item, int slot)
{
	m_Items[slot] = item;
}

int Container::findAccessableSlot(Item& item)
{
	int foundSlot = -1;
	bool foundEmptySlot = false;

	for (int i = 0; i < m_Items.size(); i++)
	{
		if (!isSlotTaken(i) && !foundEmptySlot)
		{
			foundSlot = i;
			foundEmptySlot = true;
		}
		else
		{
			if (m_Items[i].itemID == item.itemID && m_Items[i].StackSize < m_Items[i].refStruct->maxStack)
			{
				foundSlot = i;
				break;
			}
		}
	}

	return foundSlot;
}

bool Container::isSlotAccessable(int slot, Item& itemHoveredWith)
{
	if (!isSlotTaken(slot))
	{
		// Slot is empty
		return true;
	}

	if (m_Items[slot].StackSize < m_Items[slot].refStruct->maxStack)
	{
		if (m_Items[slot].itemID == itemHoveredWith.itemID)
		{
			// Slot is not full and item is off same type
			return true;
		}

		// Item in slot was of different type
		return false;
	}

	// Slot is full
	return false;
}

bool Container::isSlotSameType(int slot, Item& itemHoveredWith)
{
	return (m_Items[slot].itemID == itemHoveredWith.itemID);
}
