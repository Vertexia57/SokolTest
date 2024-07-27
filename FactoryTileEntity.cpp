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

	lost::IntVector2D worldPos = { (int)(m_Hitbox.x + m_Hitbox.w / 2.0f), (int)(m_Hitbox.y + m_Hitbox.h / 2.0f) };
	for (int x = floor(worldPos.x - 1); x < ceil(worldPos.x + m_Hitbox.w + 1); x++)
	{
		for (int y = floor(worldPos.y - 1); y < ceil(worldPos.y + m_Hitbox.h + 1); y++)
		{
			Tile* tile = g_World->getTileAt(x, y);
			tile->tileUpdate();
		}
	}
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
