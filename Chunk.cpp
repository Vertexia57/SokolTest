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
			m_Tiles.push_back(new Tile({x, y}, 0));
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

void Chunk::setTile(Tile* tile, int x, int y)
{
	delete m_Tiles[x + y * m_Width];
	m_Tiles[x + y * m_Width] = tile;
	tile->pos = { x, y };
}

void Chunk::addTileEntity(TileEntity* tileEntity, lost::Vector2D position)
{
	int localX = (position.x - chunkCoord.x * 16);
	int localY = (position.y - chunkCoord.y * 16);

	m_TileEntities.push_back(tileEntity);
	tileEntity->position = position;
	lost::Bound2D tileHitbox = tileEntity->getHitbox();
	lost::Bound2D tileBounds = { (float)localX, (float)localY, tileHitbox.w, tileHitbox.h };
	for (int y = floor(tileBounds.top); y < ceil(tileBounds.bottom); y++)
	{
		for (int x = floor(tileBounds.left); x < ceil(tileBounds.right); x++)
		{
			if (x >= 0 && x < m_Width && y >= 0 && y < m_Height)
				getTile(x, y)->addTileEntityRef(tileEntity);
		}
	}
}

void Chunk::destroyTileEntity(TileEntity* tileEntity)
{
	int localX = (tileEntity->position.x - chunkCoord.x * 16);
	int localY = (tileEntity->position.y - chunkCoord.y * 16);

	lost::Bound2D tileHitbox = tileEntity->getHitbox();
	lost::Bound2D tileBounds = { (float)localX, (float)localY, tileHitbox.w, tileHitbox.h };
	for (int y = floor(tileBounds.top); y < ceil(tileBounds.bottom); y++)
	{
		for (int x = floor(tileBounds.left); x < ceil(tileBounds.right); x++)
		{
			if (x >= 0 && x < m_Width && y >= 0 && y < m_Height)
				getTile(x, y)->removeTileEntityRef(tileEntity);
		}
	}

	for (int i = 0; i < m_TileEntities.size(); i++)
	{
		if (m_TileEntities[i] == tileEntity)
		{
			m_TileEntities.erase(m_TileEntities.begin() + i);
			break;
		}
	}
}

void Chunk::render()
{
	for (Tile* tile : m_Tiles)
		tile->render();
	for (TileEntity* tileEntity : m_TileEntities)
		tileEntity->render();
}
