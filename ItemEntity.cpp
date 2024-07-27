#include "ItemEntity.h"
#include "Player.h"
#include "World.h"
#include "ConveyerBeltTileEntity.h"

ItemEntity::ItemEntity(lost::Vector2D position, Item& item)
	: Entity(lost::Bound2D{ position.x, position.y, 20, 20 })
{
	itemCarried = item;
}

ItemEntity::ItemEntity(lost::Vector2D position, ItemRefStruct* item, int count)
	: Entity(lost::Bound2D{ position.x, position.y, 20, 20 })
{
	itemCarried = Item(item);
	itemCarried.StackSize = 1;
}

ItemEntity::~ItemEntity()
{
}

void ItemEntity::loopPosition(float worldLoopWidth)
{
	Entity::loopPosition(worldLoopWidth);
	m_WorldLoopWidth = worldLoopWidth;
}

void ItemEntity::update()
{
	Entity::update();

	lost::Vector2D actualPlayerPosition = g_PlayerPointer->getPosition();
	lost::Vector2D loopedPlayerPosition = { (actualPlayerPosition.x > getPosition().x) ? actualPlayerPosition.x - m_WorldLoopWidth : actualPlayerPosition.x + m_WorldLoopWidth, actualPlayerPosition.y};
	lost::Vector2D playerPosition = (actualPlayerPosition.dist(getPosition()) > loopedPlayerPosition.dist(getPosition())) ? loopedPlayerPosition : actualPlayerPosition;
	if (m_Following || playerPosition.dist(getPosition()) < 150.0f)
	{
		m_Following = true;
		lost::Vector2D directionVector = playerPosition - getPosition();
		directionVector.normalize();
		directionVector = directionVector * 50.0f * lost::deltaTime * fmaxf((175.0f - playerPosition.dist(getPosition())) / 25.0f, 2.0f);
		colliderData->setVelocity(directionVector.x, directionVector.y);
		colliderData->gravity = false;

		if (playerPosition.dist(getPosition()) < 20.0f)
		{
			int remaining = g_PlayerPointer->addItem(itemCarried);
			if (!remaining)
				killEntity = true;
			else
				itemCarried.StackSize = remaining;
		}
	}

	Tile* tileBelow = g_World->getTileAt((int)((colliderData->bounds.x + colliderData->bounds.w / 2.0f) / 32.0f), (int)((colliderData->bounds.y + colliderData->bounds.h / 2.0f) / 32.0f));
	for (TileEntity* entity : tileBelow->tileEntitiesWithin)
	{
		if (entity->tileType == "conveyerBelt")
		{
			ConveyerBeltTileEntity* belt = dynamic_cast<ConveyerBeltTileEntity*>(entity);
			if (belt->getEmpty(0))
			{
				belt->insertItem(itemCarried);
				killEntity = true;
			}
			break;
		}
	}
}

void ItemEntity::render(lost::Bound2D renderBounds, float worldLoopWidth)
{
	if (colliderData->bounds.inBounds(renderBounds))
	{
		lost::useImage(itemCarried.textureID);
		float imageWidth = lost::getImage(itemCarried.textureID)->width / itemCarried.refStruct->frames;
		float imageHeight = lost::getImage(itemCarried.textureID)->height / itemCarried.refStruct->variants;
		sgp_draw_textured_rect(0, { (float)transform.position.x, (float)transform.position.y, colliderData->bounds.w, colliderData->bounds.h }, { 0, imageHeight * itemCarried.variant, imageWidth, imageHeight });
	}
	else
	{
		float xOffset = (colliderData->bounds.x > renderBounds.right) ? -worldLoopWidth : worldLoopWidth;
		lost::Bound2D loopedBounds = { colliderData->bounds.x + xOffset, colliderData->bounds.y, colliderData->bounds.w, colliderData->bounds.h };
		if (loopedBounds.inBounds(renderBounds))
		{
			lost::useImage(itemCarried.textureID);
			float imageWidth = lost::getImage(itemCarried.textureID)->width / itemCarried.refStruct->frames;
			float imageHeight = lost::getImage(itemCarried.textureID)->height / itemCarried.refStruct->variants;
			sgp_draw_textured_rect(0, { loopedBounds.x, loopedBounds.y, loopedBounds.w, loopedBounds.h }, { 0, imageHeight * itemCarried.variant, imageWidth, imageHeight });
		}
	}
}
