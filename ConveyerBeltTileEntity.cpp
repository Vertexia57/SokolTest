#include "ConveyerBeltTileEntity.h"
#include "World.h"

ConveyerBeltTileEntity::ConveyerBeltTileEntity(TileEntityStruct* tileEntityRef_, uint32_t rotation)
	: TileEntity(tileEntityRef_, rotation)
{
	updates = true;

	if (!tileEntityRef->updateData->getObjectList().count("speed"))
	{
		switch (rotation)
		{
		case 0:
			relativeVelocity = { tileEntityRef->updateData->getFloat("xSpeed"), 0.0f };
			break;
		case 1:
			relativeVelocity = { 0.0f, tileEntityRef->updateData->getFloat("ySpeed") };
			break;
		case 2:
			relativeVelocity = { -tileEntityRef->updateData->getFloat("xSpeed"), 0.0f };
			break;
		case 3:
			relativeVelocity = { 0.0f, -tileEntityRef->updateData->getFloat("ySpeed") };
			break;
		}
		m_Speeds = { tileEntityRef->updateData->getFloat("xSpeed"), tileEntityRef->updateData->getFloat("ySpeed") };
	}
	else
	{
		switch (rotation)
		{
		case 0:
			relativeVelocity = { tileEntityRef->updateData->getFloat("speed"), 0.0f };
			break;
		case 1:
			relativeVelocity = { 0.0f, tileEntityRef->updateData->getFloat("speed") };
			break;
		case 2:
			relativeVelocity = { -tileEntityRef->updateData->getFloat("speed"), 0.0f };
			break;
		case 3:
			relativeVelocity = { 0.0f, -tileEntityRef->updateData->getFloat("speed") };
			break;
		}
		m_Speeds = { tileEntityRef->updateData->getFloat("speed"), tileEntityRef->updateData->getFloat("speed") };
	}

	tileType = "conveyerBelt";
	m_HasInventory = true;
}

ConveyerBeltTileEntity::~ConveyerBeltTileEntity()
{
	// Update neighboring conveyer belts, showing that this conveyer belt has been removed
	if (m_Left)
		m_Left->checkNeighbors();
	if (m_Right)
		m_Right->checkNeighbors();
	if (m_Up)
		m_Up->checkNeighbors();
	if (m_Down)
		m_Down->checkNeighbors();

	if (heldItem)
		delete heldItem;
}

void ConveyerBeltTileEntity::init()
{
	TileEntity::init();

	// Update neighboring conveyer belts and get neighboring conveyer belts
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

void ConveyerBeltTileEntity::update()
{
	TileEntity::update();

	m_Moving = false;

	// [!] TODO: Figure out if a update system that applies when a chain is moved should be used
	// [!]       Like factorio, but a more simplified version that instead just goes down the chain of conveyers
	// [!]       and tells them to move their items

	if (heldItem)
	{

		if (relativeVelocity.x != 0.0f)
		{
			if (fabsf(heldItem->y - 6.0f) < 1.0f)
			{
				heldItem->x += relativeVelocity.x * lost::deltaTime * 0.001f;
				if (!m_Right && !m_RightEntity && heldItem->x > m_Hitbox.w * 32.0f - 20.0f)
					heldItem->x = m_Hitbox.w * 32.0f - 20.0f;
				if (!m_Left && !m_LeftEntity && heldItem->x < 0.0f)
					heldItem->x = 0.0f;
			}
		}
		else
		{
			if (heldItem->x > 6)
				heldItem->x -= m_Speeds.x * lost::deltaTime * 0.001f;
			if (heldItem->x < 6)
				heldItem->x += m_Speeds.x * lost::deltaTime * 0.001f;
		}

		if (relativeVelocity.y != 0.0f)
		{
			if (fabsf(heldItem->x - 6.0f) < 1.0f)
			{
				heldItem->y += relativeVelocity.y * lost::deltaTime * 0.001f;
				if (!m_Down && !m_DownEntity && heldItem->y > m_Hitbox.h * 32.0f - 20.0f)
					heldItem->y = m_Hitbox.h * 32.0f - 20.0f;
				if (!m_Up && !m_UpEntity && heldItem->y < 0.0f)
					heldItem->y = 0.0f;
			}
		}
		else
		{
			if (heldItem->y > 6)
				heldItem->y -= m_Speeds.y * lost::deltaTime * 0.001f;
			if (heldItem->y < 6)
				heldItem->y += m_Speeds.y * lost::deltaTime * 0.001f;
		}

		checkPasses();
	}
	else
	{
		// Extract Items
		if (m_Rotation == 0 && m_LeftEntity)
		{
			Item extracted = m_LeftEntity->extractItem(1);
			if (!extracted.empty)
				insertItem(extracted);
		}
		else if (m_Rotation == 1 && m_UpEntity)
		{
			Item extracted = m_UpEntity->extractItem(1);
			if (!extracted.empty)
				insertItem(extracted);
		}
		else if (m_Rotation == 2 && m_RightEntity)
		{
			Item extracted = m_RightEntity->extractItem(1);
			if (!extracted.empty)
				insertItem(extracted);
		}
		else if (m_Rotation == 3 && m_DownEntity)
		{
			Item extracted = m_DownEntity->extractItem(1);
			if (!extracted.empty)
				insertItem(extracted);
		}
	}
}

void ConveyerBeltTileEntity::checkPasses()
{
	if (m_Right && relativeVelocity.x > 0) // Pass to other conveyer
	{
		if (!m_Right->getEmpty(0) && heldItem->x > m_Hitbox.w * 32.0f - 20.0f)
			heldItem->x = m_Hitbox.w * 32.0f - 20.0f;
		else if (heldItem->x > m_Hitbox.w * 32.0f - 20.0f && m_Right->getEmpty(0) && fabsf(heldItem->y - 6.0f) < 1)
		{
			passItem(m_Right, { 1, 0 }, 0);
			if (m_Left && m_Left->heldItem)
				m_Left->update(); // Might be hella inefficient but honestly not sure, works well-ish with chains of items
		}
	}
	else if (m_RightEntity && relativeVelocity.x > 0) // Insert into entity
	{
		if (!m_RightEntity->canInsert(heldItem->item) && heldItem->x > m_Hitbox.w * 32.0f - 20.0f)
			heldItem->x = m_Hitbox.w * 32.0f - 20.0f;
		if (heldItem->x > m_Hitbox.w * 32.0f - 20.0f && m_RightEntity->canInsert(heldItem->item) && fabsf(heldItem->y - 6.0f) < 1)
		{
			m_RightEntity->insertItem(heldItem->item);
			delete heldItem;
			heldItem = nullptr;
		}
	}

	if (m_Left && heldItem && relativeVelocity.x < 0) // Pass to other conveyer
	{
		if (!m_Left->getEmpty(2) && heldItem->x < 0.0f)
			heldItem->x = 0.0f;
		else if (heldItem->x < 0.0f && m_Left->getEmpty(2) && fabsf(heldItem->y - 6.0f) < 1)
		{
			passItem(m_Left, { -1, 0 }, 2);
			if (m_Right && m_Right->heldItem)
				m_Right->update();
		}
	}
	else if (m_LeftEntity && heldItem && relativeVelocity.x < 0) // Insert into entity
	{
		if (!m_LeftEntity->canInsert(heldItem->item) && heldItem->x < 0.0f)
			heldItem->x = 0.0f;
		if (heldItem->x < 0.0f && m_LeftEntity->canInsert(heldItem->item) && fabsf(heldItem->y - 6.0f) < 1)
		{
			m_LeftEntity->insertItem(heldItem->item);
			delete heldItem;
			heldItem = nullptr;
		}
	}

	if (m_Down && heldItem && relativeVelocity.y > 0) // Pass to other conveyer
	{
		if (!m_Down->getEmpty(1) && heldItem->y > m_Hitbox.h * 32.0f - 20.0f)
			heldItem->y = m_Hitbox.h * 32.0f - 20.0f;
		else if (heldItem->y > m_Hitbox.h * 32.0f - 20.0f && m_Down->getEmpty(1) && fabsf(heldItem->x - 6.0f) < 1)
		{
			passItem(m_Down, { 0, 1 }, 1);
			if (m_Up && m_Up->heldItem)
				m_Up->update();
		}
	}
	else if (m_DownEntity && heldItem && relativeVelocity.y > 0) // Insert into entity
	{
		if (!m_DownEntity->canInsert(heldItem->item) && heldItem->y > m_Hitbox.h * 32.0f - 20.0f)
			heldItem->y = m_Hitbox.h * 32.0f - 20.0f;
		if (heldItem->y > m_Hitbox.h * 32.0f - 20.0f && m_DownEntity->canInsert(heldItem->item) && fabsf(heldItem->x - 6.0f) < 1)
		{
			m_DownEntity->insertItem(heldItem->item);
			delete heldItem;
			heldItem = nullptr;
		}
	}

	if (m_Up && heldItem && relativeVelocity.y < 0) // Pass to other conveyer
	{
		if (!m_Up->getEmpty(3) && heldItem->y < 0.0f)
			heldItem->y = 0.0f;
		else if (heldItem->y < 0.0f && m_Up->getEmpty(3) && fabsf(heldItem->x - 6.0f) < 1)
		{
			passItem(m_Up, { 0, -1 }, 3);
			if (m_Down && m_Down->heldItem)
				m_Down->update();
		}
	}
	else if (m_UpEntity && heldItem && relativeVelocity.y < 0) // Insert into entity
	{
		if (!m_UpEntity->canInsert(heldItem->item) && heldItem->y < 0.0f)
			heldItem->y = 0.0f;
		if (heldItem->y < 0.0f && m_UpEntity->canInsert(heldItem->item) && fabsf(heldItem->x - 6.0f) < 1)
		{
			m_UpEntity->insertItem(heldItem->item);
			delete heldItem;
			heldItem = nullptr;
		}
	}
}

void ConveyerBeltTileEntity::tileUpdate()
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

void ConveyerBeltTileEntity::render()
{
	lost::useImage(tileEntityRef->texture);
	float imageWidth = lost::getImage(tileEntityRef->texture)->width / tileEntityRef->totalFrames;
	float imageHeight = lost::getImage(tileEntityRef->texture)->height / tileEntityRef->totalVariants;
	sgp_draw_textured_rect(0, { (float)position.x * 32.0f, (float)position.y * 32.0f, m_Hitbox.w * 32.0f, m_Hitbox.h * 32.0f }, { imageWidth * m_Frame, imageHeight * m_Variant, imageWidth, imageHeight });

}

void ConveyerBeltTileEntity::renderAt(lost::Vector2D pos)
{
	lost::useImage(tileEntityRef->texture);
	float imageWidth = lost::getImage(tileEntityRef->texture)->width / tileEntityRef->totalFrames;
	float imageHeight = lost::getImage(tileEntityRef->texture)->height / tileEntityRef->totalVariants;
	sgp_draw_textured_rect(0, { (float)pos.x * 32.0f, (float)pos.y * 32.0f, m_Hitbox.w * 32.0f, m_Hitbox.h * 32.0f }, { imageWidth * m_Frame, imageHeight * m_Variant, imageWidth, imageHeight });
}

void ConveyerBeltTileEntity::renderForeground()
{
	if (heldItem)
	{
		if (!heldItem->item.empty)
		{
			lost::useImage(heldItem->item.textureID);
			float imageWidth = lost::getImage(heldItem->item.textureID)->width / heldItem->item.refStruct->frames;
			float imageHeight = lost::getImage(heldItem->item.textureID)->height / heldItem->item.refStruct->variants;
			sgp_draw_textured_rect(0, { (float)position.x * 32.0f + heldItem->x, (float)position.y * 32.0f + heldItem->y, 20.0f, 20.0f }, { 0, imageHeight * heldItem->item.variant, imageWidth, imageHeight });
		}
	}
}

void ConveyerBeltTileEntity::renderForegroundAt(lost::Vector2D pos)
{
	if (heldItem)
	{
		if (!heldItem->item.empty)
		{
			lost::useImage(heldItem->item.textureID);
			float imageWidth = lost::getImage(heldItem->item.textureID)->width / heldItem->item.refStruct->frames;
			float imageHeight = lost::getImage(heldItem->item.textureID)->height / heldItem->item.refStruct->variants;
			sgp_draw_textured_rect(0, { (float)pos.x * 32.0f + heldItem->x, (float)pos.y * 32.0f + heldItem->y, 20.0f, 20.0f }, { 0, imageHeight * heldItem->item.variant, imageWidth, imageHeight });
		}
	}
}

void ConveyerBeltTileEntity::checkNeighbors()
{
	try
	{
		lost::IntVector2D worldPos = { (int)(m_Hitbox.x + m_Hitbox.w / 2.0f), (int)(m_Hitbox.y + m_Hitbox.h / 2.0f) };

		m_Left = nullptr;
		m_LeftEntity = nullptr;
		for (TileEntity* entity : g_World->getTileAt(worldPos.x - 1, worldPos.y)->tileEntitiesWithin)
		{
			if (entity->tileType == tileType)
			{
				ConveyerBeltTileEntity* cast = dynamic_cast<ConveyerBeltTileEntity*>(entity);
				if ((cast->m_Rotation == 0 || m_Rotation == 2) && !(cast->m_Rotation == 0 && m_Rotation == 2) || (cast->m_Rotation == 4))
					m_Left = cast;
				break;
			}
			else if (entity->hasInventory())
			{
				m_LeftEntity = entity;
			}
		}

		m_Right = nullptr;
		m_RightEntity = nullptr;
		for (TileEntity* entity : g_World->getTileAt(worldPos.x + 1, worldPos.y)->tileEntitiesWithin)
		{
			if (entity->tileType == tileType)
			{
				ConveyerBeltTileEntity* cast = dynamic_cast<ConveyerBeltTileEntity*>(entity);
				if ((cast->m_Rotation == 2 || m_Rotation == 0) && !(cast->m_Rotation == 2 && m_Rotation == 0) || (cast->m_Rotation == 4))
					m_Right = cast;
				break;
			}
			else if (entity->hasInventory())
			{
				m_RightEntity = entity;
			}
		}

		m_Up = nullptr;
		m_UpEntity = nullptr;
		for (TileEntity* entity : g_World->getTileAt(worldPos.x, worldPos.y - 1)->tileEntitiesWithin)
		{
			if (entity->tileType == tileType)
			{
				ConveyerBeltTileEntity* cast = dynamic_cast<ConveyerBeltTileEntity*>(entity);
				if ((cast->m_Rotation == 1 || m_Rotation == 3) && !(cast->m_Rotation == 1 && m_Rotation == 3) || (cast->m_Rotation == 4))
					m_Up = cast;
				break;
			}
			else if (entity->hasInventory())
			{
				m_UpEntity = entity;
			}
		}

		m_Down = nullptr;
		m_DownEntity = nullptr;
		for (TileEntity* entity : g_World->getTileAt(worldPos.x, worldPos.y + 1)->tileEntitiesWithin)
		{
			if (entity->tileType == tileType)
			{
				ConveyerBeltTileEntity* cast = dynamic_cast<ConveyerBeltTileEntity*>(entity);
				if ((cast->m_Rotation == 3 || m_Rotation == 1) && !(cast->m_Rotation == 3 && m_Rotation == 1) || (cast->m_Rotation == 4))
					m_Down = cast;
				break;
			}
			else if (entity->hasInventory())
			{
				m_DownEntity = entity;
			}
		}

		switch (m_Rotation)
		{
		case 0:

			if (m_Left && m_Right) { m_Variant = 26; break; }
			else if (m_Left) { m_Variant = 27; break; }
			else if (m_Right && m_Up && !m_Down) { m_Variant = 4; break; }
			else if (m_Right && m_Down && !m_Up) { m_Variant = 12; break; }
			else if (m_Up && !m_Down) { m_Variant = 6; break; }
			else if (m_Down && !m_Up) { m_Variant = 14; break; }
			else if (m_Right) { m_Variant = 25; break; }

			m_Variant = 24;
			break;
		case 1:

			if (m_Up && m_Down) { m_Variant = 2; break; }
			else if (m_Up) { m_Variant = 3; break; }
			else if (m_Down && m_Right && !m_Left) { m_Variant = 20; break; }
			else if (m_Down && m_Left && !m_Right) { m_Variant = 28; break; }
			else if (m_Right && !m_Left) { m_Variant = 22; break; }
			else if (m_Left && !m_Right) { m_Variant = 30; break; }
			else if (m_Down) { m_Variant = 1; break; }

			m_Variant = 0;
			break;
		case 2:

			if (m_Left && m_Right) { m_Variant = 18; break; }
			else if (m_Right) { m_Variant = 19; break; }
			else if (m_Left && m_Up && !m_Down) { m_Variant = 5; break; }
			else if (m_Left && m_Down && !m_Up) { m_Variant = 13; break; }
			else if (m_Up && !m_Down) { m_Variant = 7; break; }
			else if (m_Down && !m_Up) { m_Variant = 15; break; }
			else if (m_Left) { m_Variant = 17; break; }

			m_Variant = 16;

			break;
		case 3:
			if (m_Up && m_Down) { m_Variant = 10; break; }
			else if (m_Down) { m_Variant = 9; break; }
			else if (m_Up && m_Right && !m_Left) { m_Variant = 21; break; }
			else if (m_Up && m_Left && !m_Right) { m_Variant = 29; break; }
			else if (m_Right && !m_Left) { m_Variant = 23; break; }
			else if (m_Left && !m_Right) { m_Variant = 31; break; }
			else if (m_Up) { m_Variant = 11; break; }

			m_Variant = 8;
			break;
		}
	}
	catch (int error)
	{
		std::cout << "CONVERYER BELT CRASHED IN CONNECTION CHECKS\n";
	}
}

void ConveyerBeltTileEntity::insertItem(Item& item)
{
	heldItem = new ConveyerBeltItem{ item, 8, 8 };
}

bool ConveyerBeltTileEntity::canInsert(Item& item) const
{
	return heldItem == nullptr && !item.empty;
}

void ConveyerBeltTileEntity::passItem(ConveyerBeltTileEntity* other, lost::IntVector2D tileOffset, int directionFrom)
{
	other->recieveItem(heldItem, tileOffset, directionFrom);
	heldItem = nullptr;
}

void ConveyerBeltTileEntity::recieveItem(ConveyerBeltItem* item, lost::IntVector2D tileOffset, int directionFrom)
{
	heldItem = item;
	lost::IntVector2D posDiff = { tileOffset.x * 32, tileOffset.y * 32 };
	heldItem->x -= posDiff.x;
	heldItem->y -= posDiff.y;
	heldItem->directionFrom = directionFrom;
}

bool ConveyerBeltTileEntity::getEmpty(int direction) const
{
	return (heldItem == nullptr || m_Moving);
}
