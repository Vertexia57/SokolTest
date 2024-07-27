#include "JunctionTileEntity.h"
#include "World.h"

JunctionTileEntity::JunctionTileEntity(TileEntityStruct* tileEntityRef_, uint32_t rotation)
	: ConveyerBeltTileEntity(tileEntityRef_, 4)
{
	m_Variant = 0;
	m_Cooldown = 32.0f / tileEntityRef->updateData->getFloat("speed") * 1000.0f;
}

JunctionTileEntity::~JunctionTileEntity()
{
	if (m_HorizontalHeldItem)
		delete m_HorizontalHeldItem;
	if (m_VerticalHeldItem)
		delete m_VerticalHeldItem;
}

void JunctionTileEntity::update()
{
	TileEntity::update();
	if (m_HorizontalHeldItem)
		m_HTransferTime += lost::deltaTime;
	if (m_VerticalHeldItem)
		m_VTransferTime += lost::deltaTime;

	if (m_HorizontalHeldItem && m_HTransferTime >= m_Cooldown)
	{

		if (m_Left && m_HorizontalHeldItem->directionFrom == 2)
		{
			if (m_Left->getEmpty(2))
			{
				passItem(m_Left, { -1, 0 }, 2);
				return;
			}
		}
		if (m_Right && m_HorizontalHeldItem->directionFrom == 0)
		{
			if (m_Right->getEmpty(0))
			{
				passItem(m_Right, { 1, 0 }, 0);
				return;
			}
		}
	}

	if (m_VerticalHeldItem && m_VTransferTime >= m_Cooldown)
	{
		if (m_Up && m_VerticalHeldItem->directionFrom == 3)
		{
			if (m_Up->getEmpty(3))
			{
				passItem(m_Up, { 0, -1 }, 3);
				return;
			}
		}
		if (m_Down && m_VerticalHeldItem->directionFrom == 1)
		{
			if (m_Down->getEmpty(1))
			{
				passItem(m_Down, { 0, 1 }, 1);
				return;
			}
		}
	}
}

void JunctionTileEntity::tileUpdate()
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

void JunctionTileEntity::renderForeground()
{
}

void JunctionTileEntity::renderForegroundAt(lost::Vector2D pos)
{
}

inline bool JunctionTileEntity::getEmpty(int direction) const
{
	if (direction == 0 || direction == 2)
	{
		return m_HorizontalHeldItem == nullptr || m_Moving;
	}
	if (direction == 1 || direction == 3)
	{
		return m_VerticalHeldItem == nullptr || m_Moving;
	}
	return false;
}

void JunctionTileEntity::checkNeighbors()
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

void JunctionTileEntity::passItem(ConveyerBeltTileEntity* other, lost::IntVector2D tileOffset, int directionFrom)
{
	if (directionFrom == 0 || directionFrom == 2)
	{
		other->recieveItem(m_HorizontalHeldItem, tileOffset, directionFrom);
		m_HorizontalHeldItem->x = 8;
		m_HorizontalHeldItem->y = 8;
		m_HorizontalHeldItem = nullptr;
		m_HTransferTime = 0.0f;
	}
	if (directionFrom == 1 || directionFrom == 3)
	{
		other->recieveItem(m_VerticalHeldItem, tileOffset, directionFrom);
		m_VerticalHeldItem->x = 8;
		m_VerticalHeldItem->y = 8;
		m_VerticalHeldItem = nullptr;
		m_VTransferTime = 0.0f;
	}
}

void JunctionTileEntity::recieveItem(ConveyerBeltItem* item, lost::IntVector2D tileOffset, int directionFrom)
{
	if (directionFrom == 0 || directionFrom == 2)
		m_HorizontalHeldItem = item;
	if (directionFrom == 1 || directionFrom == 3)
		m_VerticalHeldItem = item;

	lost::IntVector2D posDiff = { tileOffset.x * 32, tileOffset.y * 32 };
	item->x -= posDiff.x;
	item->y -= posDiff.y;
	item->directionFrom = directionFrom;
}
