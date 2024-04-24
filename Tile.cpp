#include "Tile.h"

Tile::Tile(lost::IntVector2D position, TextureID tileTexture)
{
	pos = position;
	m_TileTexture = tileTexture;
}

Tile::~Tile()
{
}

void Tile::addTileEntityRef(TileEntity* ref)
{
	tileEntitiesWithin.push_back(ref);
}

void Tile::removeTileEntityRef(TileEntity* ref)
{
	for (int i = 0; i < tileEntitiesWithin.size(); i++)
	{
		if (tileEntitiesWithin[i] == ref)
		{
			tileEntitiesWithin.erase(tileEntitiesWithin.begin() + i);
			break;
		}
	}
}

void Tile::render()
{
	lost::useImage(m_TileTexture);
	float imageWidth = lost::getImage(m_TileTexture)->width;
	float imageHeight = lost::getImage(m_TileTexture)->height;
	sgp_draw_textured_rect(0, { (float)pos.x * 64.0f, (float)pos.y * 64.0f, 64.0f, 64.0f }, { 0, 0, imageWidth, imageHeight });
}
