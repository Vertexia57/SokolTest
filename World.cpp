#include "World.h"

World::World()
{
	m_Chunks[0] = new Chunk({ 0, 0 }, 16, 16);
	m_Chunks[1] = new Chunk({ 1, 0 }, 16, 16);
}

World::~World()
{
	for (const auto& [val, chunk] : m_Chunks)
		delete chunk;
}

void World::render()
{
	for (const auto& [val, chunk] : m_Chunks)
		chunk->renderTiles();
	for (const auto& [val, chunk] : m_Chunks)
		chunk->renderTileEntities();
	for (const auto& [val, chunk] : m_Chunks)
		chunk->renderBorders();
}

void World::addTileEntity(TileEntity* tileEntity, float x, float y)
{
	// [!] TODO: make this work when a new chunk is added on a border, making that chunk get the references to the tile entity 
	// [!]       in the other chunk aswell

	int chunkX = (int)floor(x / 16.0f);

	// Adds the tile entity to the chunk's data
	m_Chunks[chunkX]->addTileEntity(tileEntity, { x, y });

	// Finds the bounds for the hitbox in local space
	lost::Bound2D tileHitbox = tileEntity->getHitbox();

	// Loops through all covered tiles and adds the tile ref to it
	for (int y = floor(tileHitbox.top); y < ceil(tileHitbox.bottom); y++)
	{
		for (int x = floor(tileHitbox.left); x < ceil(tileHitbox.right); x++)
		{
			Tile* selectedTile = getTileAt(x, y);
			if (selectedTile)
				selectedTile->addTileEntityRef(tileEntity);
		}
	}
}

void World::destroyTileEntity(TileEntity* tileEntity)
{
	// [!] TODO: make this work when a new chunk is added on a border, making that chunk get the references to the tile entity 
	// [!]       in the other chunk aswell

	int chunkX = (int)floor(tileEntity->position.x / 16.0f);

	// Adds the tile entity to the chunk's data
	m_Chunks[chunkX]->destroyTileEntity(tileEntity);

	// Finds the bounds for the hitbox in local space
	lost::Bound2D tileHitbox = tileEntity->getHitbox();

	// Loops through all covered tiles and adds the tile ref to it
	for (int y = floor(tileHitbox.top); y < ceil(tileHitbox.bottom); y++)
	{
		for (int x = floor(tileHitbox.left); x < ceil(tileHitbox.right); x++)
		{
			Tile* selectedTile = getTileAt(x, y);
			if (selectedTile)
				selectedTile->removeTileEntityRef(tileEntity);
		}
	}

	delete tileEntity;
}

bool World::checkCanPlace(lost::Bound2D bounds, std::array<bool, 3> layers)
{
	bool taken = false;

	// Loops through all covered tiles
	for (int y = floor(bounds.top); y < ceil(bounds.bottom) && !taken; y++)
	{
		for (int x = floor(bounds.left); x < ceil(bounds.right) && !taken; x++)
		{

			// Check if any of that tiles filledLayers match up with the layers given
			Tile* tile = getTileAt(x, y);
			if (tile)
			{
				for (int i = 0; i < 3; i++)
				{
					if (layers[i] && tile->filledLayers[i])
					{
						// If they do, break out, and don't check any further
						taken = true;
						break;
					}
				}	
			}
			else
			{
				taken = true;
			}

		}
	}

	return !taken;
}

Tile* World::getTileAt(int x, int y)
{
	if (m_Chunks.count((int)floor(x / 16.0f)) > 0)
		return m_Chunks[(int)floor(x / 16.0f)]->getTile(x, y);
	return nullptr;
}
