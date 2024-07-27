#include "FactoryTileEntity.h"
#include "Player.h"
#include "World.h"

FactoryTileEntity::FactoryTileEntity(TileEntityStruct* tileEntityRef_, uint32_t rotation)
	: TileEntity(tileEntityRef_, rotation)
{
	m_CraftingGroup = tileEntityRef->updateData->getString("craftingGroup");
}

FactoryTileEntity::~FactoryTileEntity()
{
	if (m_Storage)
		delete m_Storage;
}

void FactoryTileEntity::update()
{
	TileEntity::update();

	if (m_PowerCircuit != 0xffffffff && m_CanCraft)
	{
		m_CraftingTime += lost::deltaTime * 0.001f * g_World->getPowerCircuit(m_PowerCircuit).satisfaction;
		m_Active = true;
	}
	else
	{
		m_Active = false;
	}

	if (m_CraftingTime >= m_TimeToCraft && m_CanCraft)
	{
		m_CraftingTime = 0.0f;

		// Add items to results
		for (int i = 0; i < m_SetRecipie->results.size(); i++)
		{
			RecipieRefStruct::IdCountPair& pair = m_SetRecipie->results[i];
			m_Storage->getItem(m_SetRecipie->ingredients.size() + i)->StackSize += pair.count;
		}

		m_CanCraft = true;

		// Remove Items and check if can craft again
		for (int i = 0; i < m_SetRecipie->ingredients.size(); i++)
		{
			RecipieRefStruct::IdCountPair& pair = m_SetRecipie->ingredients[i];
			m_Storage->getItem(i)->StackSize -= pair.count;

			if (m_Storage->getItem(i)->StackSize < pair.count)
				m_CanCraft = false;
		}
	}
}

void FactoryTileEntity::tileUpdate()
{
	TileEntity::tileUpdate();
}

void FactoryTileEntity::setRecipie(RecipieRefStruct* recipie)
{
	if (m_Storage)
		delete m_Storage;

	int slotReq = recipie->ingredients.size() + recipie->results.size();
	m_Storage = new Container(slotReq);

	int slotIndex = 0;
	for (RecipieRefStruct::IdCountPair& pair : recipie->ingredients)
	{
		m_Storage->lockSlot(g_ItemManager.getItemData(pair.id), slotIndex);
		slotIndex++;
	}

	for (RecipieRefStruct::IdCountPair& pair : recipie->results)
	{
		m_Storage->lockSlot(g_ItemManager.getItemData(pair.id), slotIndex);
		m_Storage->getItem(slotIndex)->output = true;
		slotIndex++;
	}

	m_TimeToCraft = recipie->timeToCraft;
	m_SetRecipie = recipie;

	for (int x = floor(m_Hitbox.x - 1); x < ceil(m_Hitbox.x + m_Hitbox.w + 1); x++)
	{
		for (int y = floor(m_Hitbox.y - 1); y < ceil(m_Hitbox.y + m_Hitbox.h + 1); y++)
		{
			Tile* tile = g_World->getTileAt(x, y);
			tile->tileUpdate();
		}
	}
}

void FactoryTileEntity::checkRecipie()
{
	if (m_SetRecipie)
	{
		m_CanCraft = true;
		for (int i = 0; i < m_SetRecipie->ingredients.size(); i++)
		{
			RecipieRefStruct::IdCountPair& pair = m_SetRecipie->ingredients[i];
			if (m_Storage->getItem(i)->StackSize < pair.count)
				m_CanCraft = false;
		}
	}
}

void FactoryTileEntity::insertItem(Item& item)
{
	if (hasInventory() && !item.empty)
		m_Storage->addItem(item);

	checkRecipie();
}

void FactoryTileEntity::mouseInteractFunction()
{
	if (lost::mouseTapped(0))
	{
		if (!lost::UIHasWindow(m_RecipieWindow))
		{
			g_PlayerPointer->openInventory({ 0.0f, 0.0f }, false);
			m_RecipieWindow = new RecipieSelectWindow({ floor(sapp_width() / 2.0f), floor(sapp_height() / 2.0f), 500, 300 }, this, m_CraftingGroup);
			m_RecipieWindow->setName("Chest");
			m_RecipieWindow->setType("chest", true);
			m_RecipieWindow->setPosition({ floor(m_RecipieWindow->getBounds().x - m_RecipieWindow->getBounds().w - 20.0f), floor(sapp_height() / 2.0f) });
			lost::addUIWindow(m_RecipieWindow);
		}
		else
		{
			m_RecipieWindow->toRemove = true;
		}
	}
}
