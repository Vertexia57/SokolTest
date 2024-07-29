#include "DistributorTileEntity.h"
#include "World.h"

DistributorTileEntity::DistributorTileEntity(TileEntityStruct* tileEntityRef_, uint32_t rotation)
	: ConveyerBeltTileEntity(tileEntityRef_, 4)
{
	m_Variant = 0;
	m_Cooldown = 32.0f / tileEntityRef->updateData->getFloat("speed") * 1000.0f;
}

DistributorTileEntity::~DistributorTileEntity()
{
	for (ConveyerBeltItem* item : m_DirectionHeldItems)
	{
		if (item)
			delete item;
	}
}

void DistributorTileEntity::update()
{
	TileEntity::update();

	for (int i = 0; i < 4; i++)
	{
		if (m_DirectionHeldItems[i])
			m_TransferTime[i] += lost::deltaTime;
		if (m_DirectionHeldItems[i] && m_TransferTime[i] >= m_Cooldown)
		{
			if (!(m_Left && !m_Output[2] && m_DirectionHeldItems[i]->directionFrom != 0) && !(m_Right && !m_Output[0] && m_DirectionHeldItems[i]->directionFrom != 2) && !(m_Up && !m_Output[3] && m_DirectionHeldItems[i]->directionFrom != 1) && !(m_Down && !m_Output[1] && m_DirectionHeldItems[i]->directionFrom != 3))
			{
				for (bool& v : m_Output)
					v = false;
			}

			if (m_Left && !m_Output[2] && m_DirectionHeldItems[i]->directionFrom != 0)
			{
				if (m_Left->getEmpty(2))
				{
					passItem(m_Left, { -1, 0 }, 2, m_DirectionHeldItems[i]);
					m_Output[2] = true;
					return;
				}
				else
				{
					m_Output[2] = true;
				}
			}
			if (m_Right && !m_Output[0] && m_DirectionHeldItems[i]->directionFrom != 2)
			{
				if (m_Right->getEmpty(0))
				{
					passItem(m_Right, { 1, 0 }, 0, m_DirectionHeldItems[i]);
					m_Output[0] = true;
					return;
				}
				else
				{
					m_Output[0] = true;
				}
			}
			if (m_Up && !m_Output[3] && m_DirectionHeldItems[i]->directionFrom != 1)
			{
				if (m_Up->getEmpty(3))
				{
					passItem(m_Up, { 0, -1 }, 3, m_DirectionHeldItems[i]);
					m_Output[3] = true;
					return;
				}
				else
				{
					m_Output[3] = true;
				}
			}
			if (m_Down && !m_Output[1] && m_DirectionHeldItems[i]->directionFrom != 3)
			{
				if (m_Down->getEmpty(1))
				{
					passItem(m_Down, { 0, 1 }, 1, m_DirectionHeldItems[i]);
					m_Output[1] = true;
					return;
				}
				else
				{
					m_Output[1] = true;
				}
			}
		}
	}
}

void DistributorTileEntity::tileUpdate()
{
	TileEntity::tileUpdate();

	checkNeighbors();
	if (m_Left)
		m_Left->checkNeighbors();
	if (m_Right)
		m_Right->checkNeighbors();
	if (m_Up)
		m_Up->checkNeighbors();
	if (m_Down)
		m_Down->checkNeighbors();
}

void DistributorTileEntity::renderForeground()
{
}

void DistributorTileEntity::renderForegroundAt(lost::Vector2D pos)
{
}

bool DistributorTileEntity::getEmpty(int direction) const
{
	return m_DirectionHeldItems[direction] == nullptr || m_Moving;
}

void DistributorTileEntity::checkNeighbors()
{
	lost::IntVector2D worldPos = { (int)(m_Hitbox.x + m_Hitbox.w / 2.0f), (int)(m_Hitbox.y + m_Hitbox.h / 2.0f) };

	m_Left = nullptr;
	for (TileEntity* entity : g_World->getTileAt(worldPos.x - 1, worldPos.y)->tileEntitiesWithin)
	{
		if (entity->tileType == tileType)
		{
			ConveyerBeltTileEntity* cast = dynamic_cast<ConveyerBeltTileEntity*>(entity);
			if (cast->getRotation() != 0)
				m_Left = cast;
			break;
		}
	}

	m_Right = nullptr;
	for (TileEntity* entity : g_World->getTileAt(worldPos.x + 1, worldPos.y)->tileEntitiesWithin)
	{
		if (entity->tileType == tileType)
		{
			ConveyerBeltTileEntity* cast = dynamic_cast<ConveyerBeltTileEntity*>(entity);
			if (cast->getRotation() != 2)
				m_Right = cast;
			break;
		}
	}

	m_Up = nullptr;
	for (TileEntity* entity : g_World->getTileAt(worldPos.x, worldPos.y - 1)->tileEntitiesWithin)
	{
		if (entity->tileType == tileType)
		{
			ConveyerBeltTileEntity* cast = dynamic_cast<ConveyerBeltTileEntity*>(entity);
			if (cast->getRotation() != 1)
				m_Up = cast;
			break;
		}
	}

	m_Down = nullptr;
	for (TileEntity* entity : g_World->getTileAt(worldPos.x, worldPos.y + 1)->tileEntitiesWithin)
	{
		if (entity->tileType == tileType)
		{
			ConveyerBeltTileEntity* cast = dynamic_cast<ConveyerBeltTileEntity*>(entity);
			if (cast->getRotation() != 3)
				m_Down = cast;
			break;
		}
	}
}

void DistributorTileEntity::passItem(ConveyerBeltTileEntity* other, lost::IntVector2D tileOffset, int directionFrom, ConveyerBeltItem*& item)
{
	m_TransferTime[item->directionFrom] = 0.0f;
	other->recieveItem(item, tileOffset, directionFrom);
	item->x = 8;
	item->y = 8;
	item = nullptr;
}

void DistributorTileEntity::recieveItem(ConveyerBeltItem* item, lost::IntVector2D tileOffset, int directionFrom)
{
	m_DirectionHeldItems[directionFrom] = item;

	lost::IntVector2D posDiff = { tileOffset.x * 32, tileOffset.y * 32 };
	item->x -= posDiff.x;
	item->y -= posDiff.y;
	item->directionFrom = directionFrom;
}
