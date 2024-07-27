#include "TileEntity.h"
#include "Random.h"
#include "World.h"

TileEntity::TileEntity(TileEntityStruct* tileEntityRef_, uint32_t rotation)
{
	m_Rotation = rotation;

	position = { 0, 0 };
	setHitbox({ 0, 0, (float)tileEntityRef_->width, (float)tileEntityRef_->height });
	tileEntityRef = tileEntityRef_;
	collidable = tileEntityRef->collidable;

	for (int i = 0; i < 3; i++)
		fillsLayers[i] = tileEntityRef_->fillsLayers[i];

	if (tileEntityRef->randomVariant)
		m_Variant = random(0, tileEntityRef->totalVariants - 1);
	else
	{
		if (tileEntityRef_->rotationVariants.size() > 0)
			m_Variant = tileEntityRef_->rotationVariants[rotation];
		else
			m_Variant = rotation;
	}

	if (tileEntityRef->building)
	{
		m_IdleConsumption = tileEntityRef->powerIdleUsage;
		m_ActiveConsumption = tileEntityRef->powerActiveUsage;
		m_BaseProduce = tileEntityRef->powerProduce;
	}
}

TileEntity::~TileEntity()
{
	g_World->tileUpdateArea(m_Hitbox);
	g_World->getPowerCircuit(m_PowerCircuit).leave(this);
}

void TileEntity::setHitbox(lost::Bound2D hitbox)
{
	m_Hitbox = hitbox;
}

void TileEntity::setPosition(lost::Vector2D position_)
{
	position = position_;
	m_Hitbox.x = position.x;
	m_Hitbox.y = position.y;
	m_Hitbox.calcSides();
}

lost::Bound2D TileEntity::getHitbox()
{
	return m_Hitbox;
}

void TileEntity::init()
{
	tileUpdate();
	g_World->tileUpdateArea(m_Hitbox);
}

void TileEntity::update()
{
	m_TileUpdated = false;

	if (tileEntityRef->animated)
	{
		m_TimeFromLastFrame += lost::deltaTime / 1000.0;
		if (m_TimeFromLastFrame > tileEntityRef->frameRate)
		{
			m_TimeFromLastFrame = 0.0;
			m_Frame = (m_Frame + 1) % tileEntityRef->totalFrames;
		}
	}
}

void TileEntity::tileUpdate()
{
	m_TileUpdated = true;

	// Check power sources
	checkPowerCircuit();
}

void TileEntity::checkPowerCircuit()
{
	if (m_ActiveConsumption > 0.0f)
	{
		uint32_t oldCircuit = m_PowerCircuit;
		m_PowerCircuit = 0xffffffff;

		for (int x = floor(m_Hitbox.x); x < ceil(m_Hitbox.x + m_Hitbox.w); x++)
		{
			for (int y = floor(m_Hitbox.y); y < ceil(m_Hitbox.y + m_Hitbox.h); y++)
			{
				Tile* tileAt = g_World->getTileAt(x, y);
				if (tileAt->getPowerCircuit() != 0xffffffff)
				{
					m_PowerCircuit = tileAt->getPowerCircuit();
				}
			}
		}

		if (oldCircuit != m_PowerCircuit)
		{
			if (m_PowerCircuit == 0xffffffff)
				g_World->getPowerCircuit(oldCircuit).leave(this);
			else
			{
				g_World->getPowerCircuit(m_PowerCircuit).join(this);
				if (oldCircuit != 0xffffffff)
					g_World->getPowerCircuit(oldCircuit).leave(this);
			}
		}
	}
}

void TileEntity::render()
{
	lost::useImage(tileEntityRef->texture);
	float imageWidth = lost::getImage(tileEntityRef->texture)->width / tileEntityRef->totalFrames;
	float imageHeight = lost::getImage(tileEntityRef->texture)->height / tileEntityRef->totalVariants;
	sgp_draw_textured_rect(0, { (float)position.x * 32.0f, (float)position.y * 32.0f, m_Hitbox.w * 32.0f, m_Hitbox.h * 32.0f }, { imageWidth * m_Frame, imageHeight * m_Variant, imageWidth, imageHeight });
}

void TileEntity::renderAt(lost::Vector2D pos)
{
	lost::useImage(tileEntityRef->texture);
	float imageWidth = lost::getImage(tileEntityRef->texture)->width / tileEntityRef->totalFrames;
	float imageHeight = lost::getImage(tileEntityRef->texture)->height / tileEntityRef->totalVariants;
	sgp_draw_textured_rect(0, { (float)pos.x * 32.0f, (float)pos.y * 32.0f, m_Hitbox.w * 32.0f, m_Hitbox.h * 32.0f }, { imageWidth * m_Frame, imageHeight * m_Variant, imageWidth, imageHeight });
}

void TileEntity::renderForeground()
{
}

void TileEntity::renderForegroundAt(lost::Vector2D pos)
{
}

void TileEntity::renderHitbox()
{
	sgp_point lines[5] = { {m_Hitbox.x * 32.0f, m_Hitbox.y * 32.0f},
						   {m_Hitbox.x * 32.0f + m_Hitbox.w * 32.0f, m_Hitbox.y * 32.0f},
						   {m_Hitbox.x * 32.0f + m_Hitbox.w * 32.0f, m_Hitbox.y * 32.0f + m_Hitbox.h * 32.0f},
						   {m_Hitbox.x * 32.0f, m_Hitbox.y * 32.0f + m_Hitbox.h * 32.0f},
						   {m_Hitbox.x * 32.0f, m_Hitbox.y * 32.0f} };

	sgp_draw_lines_strip(lines, 5);
}

void TileEntity::removeItem(Item& item, int count, bool output)
{
	if (hasInventory() && !item.empty)
		m_Storage->removeItem(item, count, output);
}

Item TileEntity::extractItem(int count, bool output)
{
	if (hasInventory())
		return m_Storage->extractItem(count, output);

	return Item();
}

void TileEntity::insertItem(Item& item)
{
	if (hasInventory() && !item.empty)
		m_Storage->addItem(item);
}

bool TileEntity::canInsert(Item& item) const
{
	if (hasInventory() && !item.empty)
		return m_Storage->findAccessableSlot(item) >= 0;
	return false;
}

void TileEntity::mouseInteractFunction()
{
}
