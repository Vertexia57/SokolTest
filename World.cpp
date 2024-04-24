#include "World.h"

World::World()
{
	m_Chunks[0] = new Chunk({ 0, 0 }, 16, 16);
}

World::~World()
{
	for (const auto& [val, chunk] : m_Chunks)
		delete chunk;
}

void World::render()
{
	for (const auto& [val, chunk] : m_Chunks)
		chunk->render();
}

void World::addTileEntity(TileEntity* tileEntity, float x, float y)
{
	m_Chunks[(int)floor(x / 16.0f)]->addTileEntity(tileEntity, { x, y });
}

void World::destroyTileEntity(TileEntity* tileEntity)
{
	m_Chunks[(int)floor(tileEntity->position.x / 16.0f)]->destroyTileEntity(tileEntity);
	delete tileEntity;
}

Tile* World::getTileAt(int x, int y)
{
	if (m_Chunks.count((int)floor(x / 16.0f)) > 0)
		return m_Chunks[(int)floor(x / 16.0f)]->getTile(x, y);
	return nullptr;
}
