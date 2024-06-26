#include "TileEntity.h"
#include "Random.h"

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
}

TileEntity::~TileEntity()
{
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
}

void TileEntity::update()
{
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

void TileEntity::mouseInteractFunction()
{
}
