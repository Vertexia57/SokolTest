#include "Chunk.h"

Chunk::Chunk(lost::IntVector2D chunkPos, int width, int height)
{
	m_Width = width;
	m_Height = height;
	chunkCoord = chunkPos;

	m_Tiles.reserve(width * height);
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
			m_Tiles.push_back(new Tile({x + chunkCoord.x * 16, y + chunkCoord.y * 16}, 0));
	}
}

Chunk::~Chunk()
{
	for (Tile* tile : m_Tiles)
		delete tile;
}

Tile* Chunk::getTile(int x, int y)
{
	int localX = (x - chunkCoord.x * 16);
	int localY = (y - chunkCoord.y * 16);

	if (localX >= 0 && localX < m_Width && localY >= 0 && localY < m_Height)
		return m_Tiles[localX + localY * m_Width];
	return nullptr;
}

Tile* Chunk::getLocalTile(int x, int y)
{
	if (x >= 0 && x < m_Width && y >= 0 && y < m_Height)
		return m_Tiles[x + y * m_Width];
	return nullptr;
}

void Chunk::setTile(Tile* tile, int x, int y)
{
	// Converts from world position to chunk position
	// [?] Should this be moved to the world to do?
	float localX = (x - chunkCoord.x * 16);
	float localY = (y - chunkCoord.y * 16);

	delete m_Tiles[localX + localY * m_Width];
	m_Tiles[localX + localY * m_Width] = tile;
	tile->pos = { x, y };
}

void Chunk::addTileEntity(TileEntity* tileEntity, lost::Vector2D position)
{
	m_TileEntities.push_back(tileEntity);
	tileEntity->setPosition(position);
}

void Chunk::destroyTileEntity(TileEntity* tileEntity)
{
	// Removes the tile entity from the chunk data
	for (int i = 0; i < m_TileEntities.size(); i++)
	{
		if (m_TileEntities[i] == tileEntity)
		{
			m_TileEntities.erase(m_TileEntities.begin() + i);
			break;
		}
	}
}

void Chunk::renderTiles()
{
	for (Tile* tile : m_Tiles)
		tile->render();
}

void Chunk::renderTileEntities()
{
	for (TileEntity* tileEntity : m_TileEntities)
		tileEntity->render();
}

void Chunk::renderBorders()
{
	sgp_set_color(1.0, 0.0, 0.0, 1.0);
	lost::clearImage();

	sgp_point lines[5] = { {chunkCoord.x * 64.0f * 16.0f, chunkCoord.y * 64.0f * 16.0f},
						   {chunkCoord.x * 64.0f * 16.0f + 64.0f * 16.0f, chunkCoord.y * 64.0f * 16.0f},
						   {chunkCoord.x * 64.0f * 16.0f + 64.0f * 16.0f, chunkCoord.y * 64.0f * 16.0f + 64.0f * 16.0f},
						   {chunkCoord.x * 64.0f * 16.0f, chunkCoord.y * 64.0f * 16.0f + 64.0f * 16.0f},
						   {chunkCoord.x * 64.0f * 16.0f, chunkCoord.y * 64.0f * 16.0f} };

	sgp_draw_lines_strip(lines, 5);
}
