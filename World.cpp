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
	for (Entity* entity : m_Entities)
		delete entity;
}

void World::worldInit()
{
	m_BorderAir = new Tile({ 0, 0 }, g_TileManager.getTileRef("air"));
	worldGenerator = new Generator("GameData\\TileData\\Generator.lua");
	m_WorldTileWidth = worldWidth * chunkWidth;
}

void World::update(lost::Bound2D renderBounds)
{
	//for (int x = floor(renderBounds.left / (chunkWidth * 32.0f)) - 1; x < ceil(renderBounds.right / (chunkWidth * 32.0f)) + 1; x++)
	//{
	//	if (m_Chunks.count(x))
	//}

	for (int i = m_Entities.size() - 1; i >= 0; i--)
	{
		m_Entities[i]->loopPosition(m_WorldTileWidth * 32.0f);
		m_Entities[i]->update();
		if (m_Entities[i]->killEntity)
		{
			delete m_Entities[i];
			m_Entities.erase(m_Entities.begin() + i);
		}
	}
}

void World::render(lost::Bound2D renderBounds)
{
	lost::bindShader(lost::getShader(0));
	for (int x = floor(renderBounds.left / (chunkWidth * 32.0f)) - 1; x < ceil(renderBounds.right / (chunkWidth * 32.0f)) + 1; x++)
	{
		if (x >= 0 && x < worldWidth)
		{
			if (m_Chunks.count(x))
			{
				m_Chunks[x]->renderTiles(renderBounds);
				m_Chunks[x]->renderTileEntities(renderBounds);
			}
			else
			{
				createChunk(x);
			}
		}
		else
		{
			int loopedX = (x + worldWidth) % worldWidth;
			if (m_Chunks.count(loopedX))
			{
				m_Chunks[loopedX]->renderTilesAt(renderBounds, x);
				m_Chunks[loopedX]->renderTileEntitiesAt(renderBounds, x);
			}
			else
			{
				createChunk(loopedX);
			}
		}
	}

	for (Entity* entity : m_Entities)
		entity->render(renderBounds, m_WorldTileWidth * 32.0f);

	lost::unbindShader();
	lost::clearImage();
	for (const auto& [val, chunk] : m_Chunks)
		chunk->renderBorders();

	ImGui::Text("Current Vertex Count: %i", m_Chunks.size() * chunkWidth * chunkHeight * 4);
}

void World::createChunk(int x)
{
	m_Chunks[x] = new Chunk({ x, 0 }, chunkWidth, chunkHeight);

	m_Chunks[x]->generateChunk(worldGenerator, this);

	for (int y = -1; y <= chunkHeight; y++)
	{
		for (int xPos = -1; xPos <= chunkWidth; xPos++)
		{
			updateTileConnections(xPos + x * chunkWidth, y);
		}
	}
}

void World::createTileEntity(TileEntityStruct* tileEntity, float x, float y)
{
	bool& requiresSupport = tileEntity->requiresSupport;
	lost::Vector2D placeLocation = { x + tileEntity->placementOffsetX, y + tileEntity->placementOffsetY };
	bool CanPlace = checkCanPlace({ placeLocation.x, placeLocation.y, tileEntity->width, tileEntity->height }, tileEntity->fillsLayers);
	if (requiresSupport && CanPlace)
		CanPlace = CanPlace && checkStable({ x + tileEntity->placementOffsetX, fmaxf(y + 1, placeLocation.y + tileEntity->height), tileEntity->width, 0.1});

	if (CanPlace)
		addTileEntity(new TileEntity(tileEntity), placeLocation.x, placeLocation.y);
}

void World::addTileEntity(TileEntity* tileEntity, float x, float y)
{
	// [!] TODO: make this work when a new chunk is added on a border, making that chunk get the references to the tile entity 
	// [!]       in the other chunk aswell

	float loopedX = loopX(x);

	int chunkX = (int)floor(loopedX / (float)chunkWidth);

	// Adds the tile entity to the chunk's data
	m_Chunks[chunkX]->addTileEntity(tileEntity, { loopedX, y });

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

	int chunkX = (int)floor(loopX(tileEntity->position.x) / (float)chunkWidth);

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

bool World::checkStable(lost::Bound2D bounds)
{
	bool notStable = false;

	// Loops through all covered tiles
	for (int y = floor(bounds.top); y < ceil(bounds.bottom) && !notStable; y++)
	{
		for (int x = floor(bounds.left); x < ceil(bounds.right) && !notStable; x++)
		{

			// Check if any of that tiles filledLayers match up with the layers given
			Tile* tile = getTileAt(x, y);
			if (tile != m_BorderAir)
			{
				if (!tile->referenceStruct->stableGround)
				{
					// If they do, break out, and don't check any further
					notStable = true;
					break;
				}
			}
			else
			{
				notStable = true;
			}

		}
	}

	return !notStable;
}

void World::setTile(TileRefStruct* tile, int x, int y)
{
	int loopedX = loopX(x);
	if (m_Chunks.count((int)floor(loopedX / (float)chunkWidth)) > 0 && y >= 0 && y < chunkHeight)
	{
		m_Chunks[(int)floor(loopedX / (float)chunkWidth)]->setTile(tile, loopedX, y);
		updateTileNeighbors(loopedX, y);
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

void World::addEntity(Entity* entity)
{
	m_Entities.push_back(entity);
}

Tile* World::getTileAt(int x, int y)
{
	if (m_Chunks.count((int)floor(loopX(x) / (float)chunkWidth)) > 0 && y >= 0 && y < chunkHeight)
		return m_Chunks[(int)floor(loopX(x) / (float)chunkWidth)]->getTile(loopX(x), y);
	return m_BorderAir;
}

int World::loopX(int x)
{
	return (x + m_WorldTileWidth) % m_WorldTileWidth;
}

float World::loopX(float x)
{
	return fmodf(x + m_WorldTileWidth, m_WorldTileWidth);
}

World* g_World = nullptr;