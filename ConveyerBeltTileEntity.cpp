#include "ConveyerBeltTileEntity.h"
#include "World.h"

ConveyerBeltTileEntity::ConveyerBeltTileEntity(TileEntityStruct* tileEntityRef_, uint32_t rotation)
	: TileEntity(tileEntityRef_, rotation)
{
	updates = true;

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

	// [!] TODO: remove the current conveyer belt texture and replace it with item pipe, using connecting textures
	// [!]       using the m_Up, m_Down, m_Left, and m_Right values

	if (heldItem)
	{

		if (relativeVelocity.x != 0.0f)
		{
			if (fabsf(heldItem->y - 6.0f) < 1.0f)
			{
				heldItem->x += relativeVelocity.x * lost::deltaTime * 0.1f;
				if (!m_Right && heldItem->x > m_Hitbox.w * 32.0f - 20.0f)
					heldItem->x = m_Hitbox.w * 32.0f - 20.0f;
				if (!m_Left && heldItem->x < 0.0f)
					heldItem->x = 0.0f;
			}
		}
		else
		{
			if (heldItem->x > 6)
				heldItem->x -= m_Speeds.x * lost::deltaTime * 0.1f;
			if (heldItem->x < 6)
				heldItem->x += m_Speeds.x * lost::deltaTime * 0.1f;
		}

		if (relativeVelocity.y != 0.0f)
		{
			if (fabsf(heldItem->x - 6.0f) < 1.0f)
			{
				heldItem->y += relativeVelocity.y * lost::deltaTime * 0.1f;
				if (!m_Down && heldItem->y > m_Hitbox.h * 32.0f - 20.0f)
					heldItem->y = m_Hitbox.h * 32.0f - 20.0f;
				if (!m_Up && heldItem->y < 0.0f)
					heldItem->y = 0.0f;
			}
		}
		else
		{
			if (heldItem->y > 6)
				heldItem->y -= m_Speeds.y * lost::deltaTime * 0.1f;
			if (heldItem->y < 6)
				heldItem->y += m_Speeds.y * lost::deltaTime * 0.1f;
		}

		checkPasses();
	}
}

void ConveyerBeltTileEntity::checkPasses()
{
	if (m_Right && relativeVelocity.x > 0)
	{
		if (!m_Right->getEmpty() && heldItem->x > m_Hitbox.w * 32.0f - 20.0f)
			heldItem->x = m_Hitbox.w * 32.0f - 20.0f;
		else if (heldItem->x > m_Hitbox.w * 32.0f - 20.0f && m_Right->getEmpty() && fabsf(heldItem->y - 6.0f) < 1)
		{
			passItem(m_Right, { 1, 0 });
			if (m_Left && m_Left->heldItem)
				m_Left->update(); // Might be hella inefficient but honestly not sure, works well-ish with chains of items
		}
	}
	if (m_Left && heldItem && relativeVelocity.x < 0)
	{
		if (!m_Left->getEmpty() && heldItem->x < 0.0f)
			heldItem->x = 0.0f;
		else if (heldItem->x < 0.0f && m_Left->getEmpty() && fabsf(heldItem->y - 6.0f) < 1)
		{
			passItem(m_Left, { -1, 0 });
			if (m_Right && m_Right->heldItem)
				m_Right->update();
		}
	}

	if (m_Down && heldItem && relativeVelocity.y > 0)
	{
		if (!m_Down->getEmpty() && heldItem->y > m_Hitbox.h * 32.0f - 20.0f)
			heldItem->y = m_Hitbox.h * 32.0f - 20.0f;
		else if (heldItem->y > m_Hitbox.h * 32.0f - 20.0f && m_Down->getEmpty() && fabsf(heldItem->x - 6.0f) < 1)
		{
			passItem(m_Down, { 0, 1 });
			if (m_Up && m_Up->heldItem)
				m_Up->update();
		}
	}
	if (m_Up && heldItem && relativeVelocity.y < 0)
	{
		if (!m_Up->getEmpty() && heldItem->y < 0.0f)
			heldItem->y = 0.0f;
		else if (heldItem->y < 0.0f && m_Up->getEmpty() && fabsf(heldItem->x - 6.0f) < 1)
		{
			passItem(m_Up, { 0, -1 });
			if (m_Down && m_Down->heldItem)
				m_Down->update();
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
		for (TileEntity* entity : g_World->getTileAt(worldPos.x - 1, worldPos.y)->tileEntitiesWithin)
		{
			if (entity->tileType == tileType)
			{
				ConveyerBeltTileEntity* cast = dynamic_cast<ConveyerBeltTileEntity*>(entity);
				if ((cast->m_Rotation == 0 || m_Rotation == 2) && !(cast->m_Rotation == 0 && m_Rotation == 2))
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
				if ((cast->m_Rotation == 2 || m_Rotation == 0) && !(cast->m_Rotation == 2 && m_Rotation == 0))
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
				if ((cast->m_Rotation == 1 || m_Rotation == 3) && !(cast->m_Rotation == 1 && m_Rotation == 3))
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
				if ((cast->m_Rotation == 3 || m_Rotation == 1) && !(cast->m_Rotation == 3 && m_Rotation == 1))
					m_Down = cast;
				break;
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

void ConveyerBeltTileEntity::passItem(ConveyerBeltTileEntity* other, lost::IntVector2D tileOffset)
{
	other->heldItem = heldItem;
	lost::IntVector2D posDiff = { tileOffset.x * 32, tileOffset.y * 32 };
	heldItem->x -= posDiff.x;
	heldItem->y -= posDiff.y;
	heldItem = nullptr;
}