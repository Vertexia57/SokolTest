#include "TileEntity.h"

TileEntity::TileEntity(lost::Vector2D position_, TextureID tileTexture, lost::Bound2D hitbox)
{
	position = position_;
	m_TileTexture = tileTexture;
	setHitbox(hitbox);
}

TileEntity::~TileEntity()
{
}

void TileEntity::setHitbox(lost::Bound2D hitbox)
{
	m_Hitbox = hitbox;
}

lost::Bound2D TileEntity::getHitbox()
{
	return m_Hitbox;
}

void TileEntity::render()
{
	lost::useImage(m_TileTexture);
	float imageWidth = lost::getImage(m_TileTexture)->width;
	float imageHeight = lost::getImage(m_TileTexture)->height;
	sgp_draw_textured_rect(0, { (float)position.x * 64.0f, (float)position.y * 64.0f, m_Hitbox.w * 64.0f, m_Hitbox.h * 64.0f }, { 0, 0, imageWidth, imageHeight });
}

void TileEntity::renderHitbox()
{
	sgp_point lines[5] = { {position.x * 64.0f + m_Hitbox.x * 64.0f, position.y * 64.0f + m_Hitbox.y * 64.0f},
						   {position.x * 64.0f + m_Hitbox.x * 64.0f + m_Hitbox.w * 64.0f, position.y * 64.0f + m_Hitbox.y * 64.0f},
						   {position.x * 64.0f + m_Hitbox.x * 64.0f + m_Hitbox.w * 64.0f, position.y * 64.0f + m_Hitbox.y * 64.0f + m_Hitbox.h * 64.0f},
						   {position.x * 64.0f + m_Hitbox.x * 64.0f, position.y * 64.0f + m_Hitbox.y * 64.0f + m_Hitbox.h * 64.0f},
						   {position.x * 64.0f + m_Hitbox.x * 64.0f, position.y * 64.0f + m_Hitbox.y * 64.0f} };

	sgp_draw_lines_strip(lines, 5);
}
