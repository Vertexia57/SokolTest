#include "ChestTileEntity.h"
#include "ItemManager.h"
#include "Lost/UIManager.h"
#include "Player.h"

ChestTileEntity::ChestTileEntity(TileEntityStruct* tileEntityRef_, uint32_t rotation)
	: TileEntity(tileEntityRef_, rotation)
{
	updates = false;
	interactable = true;
	m_Storage = new Container(tileEntityRef->slotCount);
	m_Width = tileEntityRef->updateData->getInt("invWidth");
	m_Height = tileEntityRef->updateData->getInt("invHeight");
	tileType = "chest";
}

ChestTileEntity::~ChestTileEntity()
{
	delete m_Storage;
}

void ChestTileEntity::mouseInteractFunction()
{
	if (lost::mouseTapped(0))
	{
		if (!lost::UIHasWindow(m_StorageWindow))
		{
			g_PlayerPointer->openInventory({ 0.0f, 0.0f }, false);
			m_StorageWindow = new StorageWindow({ floor(sapp_width() / 2.0f), floor(sapp_height() / 2.0f), 500, 300});
			m_StorageWindow->setName("Chest");
			m_StorageWindow->setType("chest", true);
			m_StorageWindow->bindContainer(m_Storage, m_Width, m_Height, 0, tileEntityRef->slotCount - 1);
			m_StorageWindow->setPosition({ floor(m_StorageWindow->getBounds().x - m_StorageWindow->getBounds().w - 20.0f), floor(sapp_height() / 2.0f) });
			lost::addUIWindow(m_StorageWindow);
		}
		else
		{
			m_StorageWindow->toRemove = true;
		}
	}
}
