#include "Tile.h"

Tile::Tile(lost::IntVector2D position, TileRefStruct* referenceStruct_)
{
	pos = position;
	referenceStruct = referenceStruct_;

	for (int i = 0; i < 3; i++)
		filledLayers[i] = referenceStruct->fillsLayers[i];

	empty = referenceStruct->empty;
}

Tile::~Tile()
{
}

void Tile::addTileEntityRef(TileEntity* ref)
{
	tileEntitiesWithin.push_back(ref);

	for (int i = 0; i < 3; i++)
	{
		if (ref->fillsLayers[i])
			filledLayers[i] = true;
	}

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

	for (int i = 0; i < 3; i++)
	{
		if (ref->fillsLayers[i])
			filledLayers[i] = false;
	}

}

void Tile::setTextureVariant(uint32_t textureVariant)
{
	m_TileID = textureVariant;
}

void Tile::render()
{
	if (!empty)
	{
		lost::useImage(referenceStruct->texture);
		float imageWidth = lost::getImage(referenceStruct->texture)->width;
		float imageHeight = lost::getImage(referenceStruct->texture)->height / referenceStruct->totalVariants;
		sgp_draw_textured_rect(0, { (float)pos.x * 32.0f, (float)pos.y * 32.0f, 32.0f, 32.0f }, { 0, imageHeight * m_TileID, imageWidth, imageHeight });
	}
}

void Tile::renderAt(lost::Vector2D position)
{
	if (!empty)
	{
		lost::useImage(referenceStruct->texture);
		float imageWidth = lost::getImage(referenceStruct->texture)->width;
		float imageHeight = lost::getImage(referenceStruct->texture)->height / referenceStruct->totalVariants;
		sgp_draw_textured_rect(0, { position.x * 32.0f, position.y * 32.0f, 32.0f, 32.0f }, { 0, imageHeight * m_TileID, imageWidth, imageHeight });
	}
}
