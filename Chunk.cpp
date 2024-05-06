#include "Chunk.h"
#include "Random.h"
#include "World.h"

Chunk::Chunk(lost::IntVector2D chunkPos, int width, int height)
{
	m_Width = width;
	m_Height = height;
	chunkCoord = chunkPos;

	m_Tiles.reserve(width * height);
	TileRefStruct* airTile = g_TileManager.getTileRef("air");
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			m_Tiles.push_back(new Tile({ x + chunkCoord.x * m_Width, y + chunkCoord.y * m_Height }, airTile));
		}
	}
}

Chunk::~Chunk()
{
	for (Tile* tile : m_Tiles)
		delete tile;
}

Tile* Chunk::getTile(int x, int y)
{
	int localX = (x - chunkCoord.x * m_Width);
	int localY = (y - chunkCoord.y * m_Height);

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

void Chunk::setTile(TileRefStruct* tile, int x, int y)
{
	// Converts from world position to chunk position
	// [?] Should this be moved to the world to do?
	float localX = (x - chunkCoord.x * m_Width);
	float localY = (y - chunkCoord.y * m_Height);

	std::vector<TileEntity*> oldCellTileEntityRefs = m_Tiles[localX + localY * m_Width]->tileEntitiesWithin;

	delete getLocalTile(localX, localY);
	m_Tiles[localX + localY * m_Width] = new Tile({ x, y }, tile);
	m_Tiles[localX + localY * m_Width]->tileEntitiesWithin = oldCellTileEntityRefs;
}

void Chunk::generateChunk(Generator* generator, World* parentWorld_)
{
	m_ParentWorld = parentWorld_;
	ready = false;
	generatedData = false;
	generationThread = std::thread(&Chunk::loadChunkGeneratedData, this, generator);
}

void Chunk::loadChunkGeneratedData(Generator* generator)
{
	chunkData = generator->generateChunk(chunkCoord.x, m_Width, m_Height, m_ParentWorld->worldWidth * m_Width);

	for (int y = 0; y < chunkData.height; y++)
	{
		for (int x = 0; x < chunkData.width; x++)
		{
			std::vector<TileEntity*> oldCellTileEntityRefs = m_Tiles[x + y * m_Width]->tileEntitiesWithin;
			delete getLocalTile(x, y);
			m_Tiles[x + y * m_Width] = new Tile({ x + chunkCoord.x * m_Width, y + chunkCoord.y * m_Height }, chunkData.tileMap[x + y * m_Width]);
			m_Tiles[x + y * m_Width]->tileEntitiesWithin = oldCellTileEntityRefs;
		}
	}

	for (int y = -1; y <= m_Height; y++)
	{
		for (int xPos = -1; xPos <= m_Width; xPos++)
		{
			m_ParentWorld->updateTileConnections(xPos + chunkCoord.x * m_Width, y);
		}
	}

	generatedData = true;
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

void Chunk::update()
{
	if (!ready && generatedData) 
	{
		generationThread.join();
		ready = true;
		m_ParentWorld->finishCreation(chunkCoord.x);

		int timesLooped = 0;
		for (TileEntityCreateStruct& createStruct : chunkData.tileEntities)
		{
			m_ParentWorld->createTileEntity(createStruct.tileEntityRef, createStruct.position.x + chunkCoord.x * m_Width, createStruct.position.y + chunkCoord.y * m_Height);
			timesLooped++;
		}
		fprintf(stdout, " [World::checkOutOfBoundEntities()] checked %i\n", timesLooped);
	}
}

void Chunk::renderTiles(lost::Bound2D renderBounds)
{
	for (int y = floor(renderBounds.top / 32.0f); y < ceil(renderBounds.bottom / 32.0f); y++)
	{
		for (int x = floor(renderBounds.left / 32.0f); x < ceil(renderBounds.right / 32.0f); x++)
		{
			Tile* tile = getTile(x, y);
			if (tile)
				tile->render();
		}
	}
}

void Chunk::renderTileEntities(lost::Bound2D renderBounds)
{
	for (TileEntity* tileEntity : m_TileEntities)
		tileEntity->render();
}

void Chunk::renderTilesAt(lost::Bound2D renderBounds, int ChunkRenderPos)
{
	for (int y = floor(renderBounds.top / 32.0f); y < ceil(renderBounds.bottom / 32.0f); y++)
	{
		for (int x = floor(renderBounds.left / 32.0f); x < ceil(renderBounds.right / 32.0f); x++)
		{
			int localX = (x - ChunkRenderPos * m_Width);
			int localY = (y - 0 * m_Height);

			Tile* tile = getLocalTile(localX, localY);
			if (tile)
				tile->renderAt({ (float)(localX + ChunkRenderPos * m_Width), (float)localY });
		}
	}
}

void Chunk::renderTileEntitiesAt(lost::Bound2D renderBounds, int ChunkRenderPos)
{
	for (TileEntity* tileEntity : m_TileEntities)
	{
		lost::Vector2D tileEntityPos = tileEntity->position;
		tileEntityPos.x -= m_Width * (chunkCoord.x - ChunkRenderPos);
		tileEntity->renderAt(tileEntityPos);
	}
}

std::vector<TileEntity*>& Chunk::getTileEntities()
{
	return m_TileEntities;
}

void Chunk::renderBorders()
{
	sgp_set_color(1.0, 0.0, 0.0, 1.0);
	lost::clearImage();

	sgp_point lines[5] = { {chunkCoord.x * 32.0f * m_Width, chunkCoord.y * 32.0f * m_Height},
						   {chunkCoord.x * 32.0f * m_Width + 32.0f * m_Width, chunkCoord.y * 32.0f * m_Height},
						   {chunkCoord.x * 32.0f * m_Width + 32.0f * m_Width, chunkCoord.y * 32.0f * m_Height + 32.0f * m_Height},
						   {chunkCoord.x * 32.0f * m_Width, chunkCoord.y * 32.0f * m_Height + 32.0f * m_Height},
						   {chunkCoord.x * 32.0f * m_Width, chunkCoord.y * 32.0f * m_Height} };

	sgp_draw_lines_strip(lines, 5);
}
