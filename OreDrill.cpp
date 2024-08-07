#include "OreDrill.h"
#include "World.h"
#include "ItemManager.h"

OreDrillEntity::OreDrillEntity(TileEntityStruct* tileEntityRef_, uint32_t rotation)
	: TileEntity(tileEntityRef_, rotation)
{
	updates = true;
	interactable = false;
	m_Storage = new Container(1);

	m_MiningTime = tileEntityRef->updateData->getFloat("timePerMine") * 1000.0f;

	tileType = "miner";
}

OreDrillEntity::~OreDrillEntity()
{
	delete m_Storage;
}

void OreDrillEntity::init()
{
	TileEntity::init();
	m_SearchForOres();
}

void OreDrillEntity::update()
{
	TileEntity::update();

	if (m_Storage->getItem(0)->StackSize > 0)
	{
		if (m_RightEntity)
		{
			if (m_RightEntity->canInsert(*m_Storage->getItem(0)))
			{
				if (m_Storage->getItem(0)->refStruct->randomVariant)
					m_Storage->getItem(0)->variant = random(0, m_Storage->getItem(0)->refStruct->variants - 1);
				m_RightEntity->insertItem(*m_Storage->getItem(0));
				m_Storage->getItem(0)->StackSize = 0;
				m_Storage->getItem(0)->locked = false;
				m_Storage->getItem(0)->output = false;
			}
		}

		if (m_LeftEntity)
		{
			if (m_LeftEntity->canInsert(*m_Storage->getItem(0)))
			{
				if (m_Storage->getItem(0)->refStruct->randomVariant)
					m_Storage->getItem(0)->variant = random(0, m_Storage->getItem(0)->refStruct->variants - 1);
				m_LeftEntity->insertItem(*m_Storage->getItem(0));
				m_Storage->getItem(0)->StackSize = 0;
				m_Storage->getItem(0)->locked = false;
				m_Storage->getItem(0)->output = false;
			}
		}
	}
	
	if (m_PowerCircuit != 0xffffffff)
		m_TimeSinceLastMine += lost::deltaTime * g_World->getPowerCircuit(m_PowerCircuit).satisfaction;

	if (m_TimeSinceLastMine >= m_MiningTime)
	{
		if (drillTile && !drillTile->empty && m_Storage->getItem(0)->StackSize == 0)
		{
			m_TimeSinceLastMine = 0.0f;
			m_Storage->addItem(g_ItemManager.getItemData(drillTile->referenceStruct->extraData->getString("mineProduce")), 1);
			(*drillTileCountRef)--;
			if (*drillTileCountRef == 0)
			{
				g_World->setTile(g_TileManager.getTileRef(drillTile->referenceStruct->extraData->getString("exhaustTile")), drillLocation.x, drillLocation.y);
				drillTile = nullptr;
				m_SearchForOres();
			}
		}
	}
	else
	{
		if (!drillTile || m_Storage->getItem(0)->StackSize != 0)
			m_Active = false;
		else
			m_Active = true;
	}
}

void OreDrillEntity::tileUpdate()
{
	TileEntity::tileUpdate();

	lost::IntVector2D worldPos = { (int)(m_Hitbox.x + m_Hitbox.w / 2.0f), (int)(m_Hitbox.y + m_Hitbox.h / 2.0f) };

	m_LeftEntity = nullptr;
	for (TileEntity* entity : g_World->getTileAt(worldPos.x - 2, worldPos.y)->tileEntitiesWithin)
	{
		if (entity->hasInventory())
		{
			m_LeftEntity = entity;
			break;
		}
	}

	m_RightEntity = nullptr;
	for (TileEntity* entity : g_World->getTileAt(worldPos.x + 2, worldPos.y)->tileEntitiesWithin)
	{
		if (entity->hasInventory())
		{
			m_RightEntity = entity;
			break;
		}
	}
}

void OreDrillEntity::m_SearchForOres()
{
	int minX = (int)floor(m_Hitbox.x - 1.0f);
	int maxX = (int)ceil(m_Hitbox.x + m_Hitbox.w);
	int maxDepth = min((int)ceil(m_Hitbox.y + m_Hitbox.h) + 30, g_World->chunkHeight - 1);
	int minDepth = (int)ceil(m_Hitbox.y + m_Hitbox.h);

	if (drillLocation.y <= position.y)
	{
		drillLocation = { maxX, minDepth };
	}

	drillTile = nullptr;
	int yLayerEmpty = 0;

	while (drillLocation.y > position.y)
	{
		Tile* focussedTile = g_World->getTileAt(drillLocation.x, drillLocation.y);
		if (focussedTile->extraLocalData != nullptr)
		{
			// Has Extra data
			if (focussedTile->referenceStruct->extraData->getObjectList().count("mineProduce"))
			{
				// Has Mining data
				drillTile = focussedTile;
				m_Storage->lockSlot(g_ItemManager.getItemData(focussedTile->referenceStruct->extraData->getString("mineProduce")), 0);
				m_Storage->getItem(0)->output = true;
				break;
			}
		}
		else if (focussedTile->empty)
		{
			yLayerEmpty++;
			if (yLayerEmpty > 2)
				break;
		}

		drillLocation.x--;
		if (drillLocation.x < minX)
		{
			drillLocation.x = maxX;
			drillLocation.y++;
			yLayerEmpty = 0;
		}
	}

	if (drillTile)
		drillTileCountRef = &(drillTile->extraLocalData->getInt("holdingOre"));
}
