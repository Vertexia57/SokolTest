#include "ConveyerBeltTileEntity.h"
#include "World.h"

ConveyerBeltTileEntity::ConveyerBeltTileEntity(TileEntityStruct* tileEntityRef_)
	: TileEntity(tileEntityRef_)
{
	updates = true;
	relativeVelocity = { tileEntityRef->updateData->getFloat("xSpeed"), 0.0f };
	tileType = "conveyerBelt";
}

ConveyerBeltTileEntity::~ConveyerBeltTileEntity()
{
	// Update neighboring conveyer belts, showing that this conveyer belt has been removed
	if (m_Left)
		m_Left->checkNeighbors();
	if (m_Right)
		m_Right->checkNeighbors();
}

void ConveyerBeltTileEntity::init()
{
	// Update neighboring conveyer belts and get neighboring conveyer belts
	checkNeighbors();
	if (m_Left)
		m_Left->checkNeighbors();
	if (m_Right)
		m_Right->checkNeighbors();
}

void ConveyerBeltTileEntity::update()
{
	m_Moving = false;

	// [!] TODO: Figure out if you should continue using this system, or should replace it.
	// [!]       This has a problem where each item "trickles" down, making it staggered and less efficient

	if (heldItem)
	{
		heldItem->x += relativeVelocity.x * lost::deltaTime * 0.1f;

		if (!m_Right && heldItem->x > m_Hitbox.w * 32.0f - 20.0f)
			heldItem->x = m_Hitbox.w * 32.0f - 20.0f;
		if (!m_Left && heldItem->x < 0.0f)
			heldItem->x = 0.0f;

		if (m_Right)
		{
			if (!m_Right->getEmpty() && heldItem->x > m_Hitbox.w * 32.0f - 20.0f)
				heldItem->x = m_Hitbox.w * 32.0f - 20.0f;
			if (heldItem->x > m_Hitbox.w * 32.0f && m_Right->getEmpty())
				passItem(m_Right, { 1, 0 });
		}
		if (m_Left && heldItem)
		{
			if (!m_Left->getEmpty() && heldItem->x < 0.0f)
				heldItem->x = 0.0f;
			if (heldItem->x < -20 && m_Left->getEmpty())
				passItem(m_Left, { -1, 0});
		}
	}
}

void ConveyerBeltTileEntity::render()
{
	lost::useImage(tileEntityRef->texture);
	float imageWidth = lost::getImage(tileEntityRef->texture)->width;
	float imageHeight = lost::getImage(tileEntityRef->texture)->height / tileEntityRef->totalVariants;
	sgp_draw_textured_rect(0, { (float)position.x * 32.0f, (float)position.y * 32.0f, m_Hitbox.w * 32.0f, m_Hitbox.h * 32.0f }, { 0, imageHeight * m_Variant, imageWidth, imageHeight });

	if (heldItem)
	{
		lost::useImage(heldItem->item.textureID);
		imageWidth = lost::getImage(heldItem->item.textureID)->width / heldItem->item.refStruct->frames;
		imageHeight = lost::getImage(heldItem->item.textureID)->height / heldItem->item.refStruct->variants;
		sgp_draw_textured_rect(0, { (float)position.x * 32.0f + heldItem->x, (float)position.y * 32.0f - 20.0f, 20.0f, 20.0f }, { 0, imageHeight * heldItem->item.variant, imageWidth, imageHeight });
	}
}

void ConveyerBeltTileEntity::renderAt(lost::Vector2D pos)
{
	lost::useImage(tileEntityRef->texture);
	float imageWidth = lost::getImage(tileEntityRef->texture)->width;
	float imageHeight = lost::getImage(tileEntityRef->texture)->height / tileEntityRef->totalVariants;
	sgp_draw_textured_rect(0, { (float)pos.x * 32.0f, (float)pos.y * 32.0f, m_Hitbox.w * 32.0f, m_Hitbox.h * 32.0f }, { 0, imageHeight * m_Variant, imageWidth, imageHeight });

	if (heldItem)
	{
		lost::useImage(heldItem->item.textureID);
		imageWidth = lost::getImage(heldItem->item.textureID)->width / heldItem->item.refStruct->frames;
		imageHeight = lost::getImage(heldItem->item.textureID)->height / heldItem->item.refStruct->variants;
		sgp_draw_textured_rect(0, { (float)pos.x * 32.0f + heldItem->x, (float)pos.y * 32.0f - 20.0f, 20.0f, 20.0f }, { 0, imageHeight * heldItem->item.variant, imageWidth, imageHeight });
	}
}

void ConveyerBeltTileEntity::checkNeighbors()
{
	lost::IntVector2D worldPos = { (int)(m_Hitbox.x + m_Hitbox.w / 2.0f), (int)(m_Hitbox.y + m_Hitbox.h / 2.0f) };

	m_Left = nullptr;
	for (TileEntity* entity : g_World->getTileAt(worldPos.x - 1, worldPos.y)->tileEntitiesWithin)
	{
		if (entity->tileType == tileType)
		{
			m_Left = dynamic_cast<ConveyerBeltTileEntity*>(entity);
			break;
		}
	}

	m_Right = nullptr;
	for (TileEntity* entity : g_World->getTileAt(worldPos.x + 1, worldPos.y)->tileEntitiesWithin)
	{
		if (entity->tileType == tileType)
		{
			m_Right = dynamic_cast<ConveyerBeltTileEntity*>(entity);
			break;
		}
	}
}

void ConveyerBeltTileEntity::addItem(Item item)
{
	heldItem = new ConveyerBeltItem{ item, 8 };
}

void ConveyerBeltTileEntity::passItem(ConveyerBeltTileEntity* other, lost::IntVector2D tileOffset)
{
	other->heldItem = heldItem;
	int posDiff = tileOffset.x * 32.0f;
	heldItem->x -= posDiff;
	heldItem = nullptr;
}
