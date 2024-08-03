#include "HubTileEntity.h"
#include "Player.h"

HubTileEntity::HubTileEntity(TileEntityStruct* tileEntityRef_, uint32_t rotation)
	: TileEntity(tileEntityRef_, rotation)
{
	updates = false;
	interactable = false;
	m_Storage = new Container(21);
	tileType = "hub";

	Item item = Item(g_ItemManager.getItemData("ironIngot"));
	item.StackSize = 500;

	insertItem(item);

	g_PlayerPointer->setConnectedHub(this);
}

HubTileEntity::~HubTileEntity()
{
	g_PlayerPointer->setConnectedHub(nullptr);
}

Item HubTileEntity::extractItem(int count, bool output)
{
	return Item();
}

void HubTileEntity::insertItem(Item& item)
{
	item.maxStackSize = 4000;
	if (hasInventory() && !item.empty)
		m_Storage->addItem(item);
}
