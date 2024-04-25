#include "World.h"

World::World()
{
}

World::~World()
{
	for (const auto& [val, chunk] : m_Chunks)
		delete chunk;
	delete m_BorderAir;
	delete worldGenerator;
}

void World::worldInit()
{
	m_BorderAir = new Tile({ 0, 0 }, g_TileManager.getTileRef("air"));
	worldGenerator = new Generator("TileData\\Generator.lua");
}

void World::render(lost::Bound2D renderBounds)
{
	lost::bindShader(lost::getShader(0));
	for (const auto& [val, chunk] : m_Chunks)
		chunk->renderTiles(renderBounds);
	for (const auto& [val, chunk] : m_Chunks)
		chunk->renderTileEntities(renderBounds);
	lost::unbindShader();
	lost::clearImage();
	//for (const auto& [val, chunk] : m_Chunks)
	//	chunk->renderBorders();
}

void World::createChunk(int x)
{
	m_Chunks[x] = new Chunk({ x, 0 }, chunkWidth, chunkHeight);

	m_Chunks[x]->generateChunk(worldGenerator);

	for (int y = -1; y <= chunkHeight; y++)
	{
		for (int xPos = -1; xPos <= chunkWidth; xPos++)
		{
			updateTileConnections(xPos + x * chunkWidth, y);
		}
	}
}

void World::addTileEntity(TileEntity* tileEntity, float x, float y)
{
	// [!] TODO: make this work when a new chunk is added on a border, making that chunk get the references to the tile entity 
	// [!]       in the other chunk aswell

	int chunkX = (int)floor(x / (float)chunkWidth);

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

	int chunkX = (int)floor(tileEntity->position.x / (float)chunkWidth);

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
			if (tile != m_BorderAir)
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

void World::setTile(TileRefStruct* tile, int x, int y)
{
	if (m_Chunks.count((int)floor(x / (float)chunkWidth)) > 0 && y >= 0 && y < chunkHeight)
	{
		m_Chunks[(int)floor(x / (float)chunkWidth)]->setTile(tile, x, y);
		updateTileNeighbors(x, y);
	}
}

void World::updateTileConnections(int x, int y)
{
	Tile* thisTile = getTileAt(x, y);
	if (thisTile != m_BorderAir)
	{

		if (thisTile->referenceStruct->connectionType != "none")
		{

			std::array<int, 9> currentMatrix{ 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			for (int xOff = -1; xOff <= 1; xOff++)
			{
				for (int yOff = -1; yOff <= 1; yOff++)
				{
					Tile* offsetTile = getTileAt(x + xOff, y + yOff);
					if (offsetTile->referenceStruct->connectionType == "none")
					{
						currentMatrix[xOff + yOff * 3 + 4] = 1;
					}
					else
					{
						if (thisTile->referenceStruct->connectsTo.count(offsetTile->referenceStruct->connectionGroup))
							currentMatrix[xOff + yOff * 3 + 4] = 2;
						else
							currentMatrix[xOff + yOff * 3 + 4] = 1;
					}
				}
			}

			std::vector<ConnectionMatrix>* matricies = g_TileManager.getConnectionData(thisTile->referenceStruct->connectionType)->getMatricies();

			int finalID = 0;
			bool matched = false;

			for (int i = 0; i < matricies->size(); i++)
			{
				ConnectionMatrix selectedMatrix = matricies->at(i);

				matched = true;
				for (int j = 0; j < 9; j++)
				{
					if (selectedMatrix.connectionMatrix[j] == 2 && currentMatrix[j] != 2)
					{
						matched = false;
						break;
					}
					if (selectedMatrix.connectionMatrix[j] == 1 && currentMatrix[j] != 1)
					{
						matched = false;
						break;
					}
				}

				if (matched)
				{
					finalID = selectedMatrix.id;
					break;
				}
			}

			thisTile->setTextureVariant(finalID);

		}
	}
}

void World::updateTileNeighbors(int x, int y)
{
	for (int xOff = -1; xOff <= 1; xOff++)
	{
		for (int yOff = -1; yOff <= 1; yOff++)
		{
			updateTileConnections(x + xOff, y + yOff);
		}
	}
}

Tile* World::getTileAt(int x, int y)
{
	if (m_Chunks.count((int)floor(x / (float)chunkWidth)) > 0 && y >= 0 && y < chunkHeight)
		return m_Chunks[(int)floor(x / (float)chunkWidth)]->getTile(x, y);
	return m_BorderAir;
}
