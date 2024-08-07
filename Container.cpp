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
				int amountToFill = (m_Items[slot].StackSize + count > m_Items[slot].maxStackSize) ? m_Items[slot].maxStackSize - m_Items[slot].StackSize : count;
				m_Items[slot].StackSize += amountToFill;
				count -= amountToFill;
			}
			else
			{
				m_Items[slot] = item;
				count = 0;
				break;
			}

			slot = findAccessableSlot(item);

			if (slot == -1)
				break;
		}
	}

	return count;
}

int Container::countItem(ItemRefStruct* refStruct, bool output)
{
	int count = 0;
	for (Item& i : m_Items)
	{
		if (i.itemID == refStruct->itemID && i.output == output)
			count += i.StackSize;
	}
	return count;
}

int Container::countItem(Item item, bool output)
{
	int count = 0;
	for (Item& i : m_Items)
	{
		if (i.itemID == item.itemID && i.output == output)
			count += i.StackSize;
	}
	return count;
}

void Container::removeItem(ItemRefStruct* refStruct, int count, bool output)
{
	for (Item& i : m_Items)
	{
		if (i.itemID == refStruct->itemID && i.output == output && i.StackSize > 0)
		{
			int amountToRemove = (i.StackSize > count) ? count : i.StackSize;
			count -= amountToRemove;
			i.StackSize -= amountToRemove;

			if (!i.locked && i.StackSize <= 0) // If the slot's stack size is zero, clear the slot
				i = Item();
		}
	}
}

void Container::removeItem(Item item, int count, bool output)
{
	for (Item& i : m_Items)
	{
		if (i.itemID == item.itemID && i.output == output && i.StackSize > 0)
		{
			int amountToRemove = (i.StackSize > count) ? count : i.StackSize;
			count -= amountToRemove;
			i.StackSize -= amountToRemove;

			if (!i.locked) // If the slot's stack size is zero, clear the slot
				i = Item();
		}
	}
}

Item Container::extractItem(int count, bool output)
{
	Item ret = Item();

	for (Item& i : m_Items)
	{
		if (i.output == output && i.StackSize > 0)
		{
			int amountToRemove = (i.StackSize > count) ? count : i.StackSize;
			count -= amountToRemove;
			i.StackSize -= amountToRemove;

			ret = i;
			ret.StackSize = amountToRemove;
			ret.locked = false;
			ret.output = false;

			if (!i.locked) // If the slot's stack size is zero, clear the slot
				i = Item();
		}
	}

	return ret;
}

int Container::addItemToSlot(Item& item, int slot)
{
	int count = item.StackSize;

	if (isSlotTaken(slot))
	{

		int amountToFill = (m_Items[slot].StackSize + count > m_Items[slot].maxStackSize) ? m_Items[slot].maxStackSize - m_Items[slot].StackSize : count;
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

void Container::removeItem(int slot)
{
	m_Items[slot] = Item();
}

void Container::lockSlot(Item lockedItem, int slot)
{
	lockedItem.StackSize = 0;
	lockedItem.locked = true;
	m_Items[slot] = lockedItem;
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
			if (m_Items[i].itemID == item.itemID && m_Items[i].StackSize < m_Items[i].maxStackSize)
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

	if (m_Items[slot].StackSize < m_Items[slot].maxStackSize)
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
