#include "FactoryTileEntity.h"
#include "Player.h"
#include "World.h"

FactoryTileEntity::FactoryTileEntity(TileEntityStruct* tileEntityRef_, uint32_t rotation)
	: TileEntity(tileEntityRef_, rotation)
{
	m_CraftingGroup = tileEntityRef->updateData->getString("craftingGroup");
	m_Speed = tileEntityRef->updateData->getFloat("speed");
}

FactoryTileEntity::~FactoryTileEntity()
{
	if (lost::UIHasWindow(m_RecipieWindow))
		m_RecipieWindow->toRemove = true;
	if (m_Storage)
		delete m_Storage;
}

void FactoryTileEntity::update()
{
	TileEntity::update();

	if (m_PowerCircuit != 0xffffffff && m_CanCraft)
	{
		m_CraftingTime += lost::deltaTime * 0.001f * g_World->getPowerCircuit(m_PowerCircuit).satisfaction * m_Speed;
		m_Active = true;
	}
	else
	{
		m_Active = false;
	}

	if (m_CraftingTime >= m_TimeToCraft && m_CanCraft && m_SetRecipie)
	{
		m_CraftingTime = 0.0f;

		m_CanCraft = true;

		// Add items to results
		for (int i = 0; i < m_SetRecipie->results.size(); i++)
		{
			IdCountPair& pair = m_SetRecipie->results[i];
			m_Storage->getItem(m_SetRecipie->ingredients.size() + i)->StackSize += pair.count;

			if (m_Storage->getItem(m_SetRecipie->ingredients.size() + i)->StackSize + pair.count > m_Storage->getItem(m_SetRecipie->ingredients.size() + i)->maxStackSize)
				m_CanCraft = false;
		}

		// Remove Items and check if can craft again
		for (int i = 0; i < m_SetRecipie->ingredients.size(); i++)
		{
			IdCountPair& pair = m_SetRecipie->ingredients[i];
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

void FactoryTileEntity::render()
{
	TileEntity::render();

	if (m_SetRecipie)
	{
		TextureID icon = m_SetRecipie->icon;
		if (icon == -1)
			icon = g_ItemManager.getItemData(m_SetRecipie->results[0].id)->textureID;

		lost::useImage(icon);
		float imageWidth = lost::getImage(icon)->width;
		float imageHeight = lost::getImage(icon)->height;
			
		sgp_draw_textured_rect(0, { (float)position.x * 32.0f + m_Hitbox.w * 16.0f - 32.0f, (float)position.y * 32.0f + m_Hitbox.h * 16.0f - 32.0f, 64.0f, 64.0f }, { imageWidth * m_Frame, imageHeight * m_Variant, imageWidth, imageHeight });
	}
}

void FactoryTileEntity::renderAt(lost::Vector2D pos)
{
	TileEntity::renderAt(pos);

	if (m_SetRecipie)
	{
		TextureID icon = m_SetRecipie->icon;
		if (icon == -1)
			icon = g_ItemManager.getItemData(m_SetRecipie->results[0].id)->textureID;

		lost::useImage(icon);
		float imageWidth = lost::getImage(icon)->width;
		float imageHeight = lost::getImage(icon)->height;

		sgp_draw_textured_rect(0, { (float)pos.x * 32.0f + m_Hitbox.w * 16.0f - 32.0f, (float)pos.y * 32.0f + m_Hitbox.h * 16.0f - 32.0f, 64.0f, 64.0f }, { imageWidth * m_Frame, imageHeight * m_Variant, imageWidth, imageHeight });
	}
}

void FactoryTileEntity::setRecipie(RecipieRefStruct* recipie)
{
	if (m_Storage)
		delete m_Storage;

	if (recipie != nullptr)
	{

		int slotReq = recipie->ingredients.size() + recipie->results.size();
		m_Storage = new Container(slotReq);

		int slotIndex = 0;
		for (IdCountPair& pair : recipie->ingredients)
		{
			m_Storage->lockSlot(g_ItemManager.getItemData(pair.id), slotIndex);
			slotIndex++;
		}

		for (IdCountPair& pair : recipie->results)
		{
			m_Storage->lockSlot(g_ItemManager.getItemData(pair.id), slotIndex);
			m_Storage->getItem(slotIndex)->output = true;
			slotIndex++;
		}

		m_TimeToCraft = recipie->timeToCraft;
	}
	else
	{
		m_Storage = nullptr;
	}

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

RecipieRefStruct* FactoryTileEntity::getRecipie() const
{
	return m_SetRecipie;
}

void FactoryTileEntity::checkRecipie()
{
	if (m_SetRecipie)
	{
		m_CanCraft = true;
		for (int i = 0; i < m_SetRecipie->ingredients.size(); i++)
		{
			IdCountPair& pair = m_SetRecipie->ingredients[i];
			if (m_Storage->getItem(i)->StackSize < pair.count)
				m_CanCraft = false;
		}
		for (int i = 0; i < m_SetRecipie->results.size(); i++)
		{
			IdCountPair& pair = m_SetRecipie->results[i];
			if (m_Storage->getItem(i + m_SetRecipie->ingredients.size())->StackSize + pair.count > m_Storage->getItem(i + m_SetRecipie->ingredients.size())->maxStackSize)
				m_CanCraft = false;
		}
	}
}

Item FactoryTileEntity::extractItem(int count, bool output)
{
	Item ret = Item();

	if (hasInventory())
		ret = m_Storage->extractItem(count, output);

	checkRecipie();

	return ret;
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
			//g_PlayerPointer->openInventory({ 0.0f, 0.0f }, false);
			m_RecipieWindow = new RecipieSelectWindow({ floor(sapp_width() / 2.0f), floor(sapp_height() / 2.0f), 500, 300 }, this, m_CraftingGroup);
			m_RecipieWindow->setName("Select Recipie");
			m_RecipieWindow->setType("recipie", true);
			m_RecipieWindow->setPosition({ floor(m_RecipieWindow->getBounds().x - m_RecipieWindow->getBounds().w / 2.0f), floor(sapp_height() / 2.0f) });
			lost::addUIWindow(m_RecipieWindow);
		}
		else
		{
			m_RecipieWindow->toRemove = true;
			m_RecipieWindow = nullptr;
		}
	}
}
