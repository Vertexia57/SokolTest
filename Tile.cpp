#include "Tile.h"

Tile::Tile(lost::IntVector2D position, TileRefStruct* referenceStruct_)
{
	pos = position;
	referenceStruct = referenceStruct_;

	for (int i = 0; i < 3; i++)
		filledLayers[i] = referenceStruct->fillsLayers[i];

	empty = referenceStruct->empty;

	// REALLY unsure about this, adds a lot of extra memory usage and fucks with cold memory
	if (referenceStruct->extraLocalData)
		extraLocalData = referenceStruct->extraLocalData->createCopy();
}

Tile::~Tile()
{
	delete extraLocalData;
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

void Tile::tileUpdate()
{
	for (TileEntity* entity : tileEntitiesWithin)
	{
		if (!entity->beenTileUpdated())
			entity->tileUpdate();
	}
}

void Tile::forceTileUpdate()
{
	for (TileEntity* entity : tileEntitiesWithin)
	{
		entity->tileUpdate();
	}
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
	else if (m_PowerCircuit != 0xffffffff)
	{
		lost::clearImage();
		sgp_set_color(0.4f, 0.8f, 1.0f, 0.1f);
		sgp_draw_filled_rect((float)pos.x * 32.0f, (float)pos.y * 32.0f, 32.0f, 32.0f);
		sgp_set_color(1.0f, 1.0f, 1.0f, 1.0f);
		//lost::renderTextPro(std::to_string(m_PowerCircuit), renPos, 0.2, LOST_TEXT_ALIGN_MIDDLE, LOST_TEXT_ALIGN_MIDDLE);
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
	else if (m_PowerCircuit != 0xffffffff)
	{
		lost::clearImage();
		sgp_set_color(0.4f, 0.8f, 1.0f, 0.1f);
		sgp_draw_filled_rect((float)position.x * 32.0f, (float)position.y * 32.0f, 32.0f, 32.0f);
		sgp_set_color(1.0f, 1.0f, 1.0f, 1.0f);
		//lost::renderTextPro(std::to_string(m_PowerCircuit), renPos, 0.2, LOST_TEXT_ALIGN_MIDDLE, LOST_TEXT_ALIGN_MIDDLE);
	}
}

void Tile::setPowerCircuit(uint32_t id)
{
	m_PowerCircuit = id;
}

void Tile::joinPowerCircuit(uint32_t id)
{
	m_PowerCircuit = id;
	m_ConnectedPowerSources++;
}

void Tile::leavePowerCircuit()
{
	m_ConnectedPowerSources--;
	if (m_ConnectedPowerSources == 0)
		m_PowerCircuit = 0xffffffff;
}

bool Tile::getStable() const
{
	if (referenceStruct->stableGround)
		return true;

	for (TileEntity* entity : tileEntitiesWithin)
	{
		if (entity->stable)
			return true;
	}

	return false;
}
