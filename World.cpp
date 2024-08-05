#include "World.h"
#include "PowerConduit.h"

World::World()
{
	m_Background = new StarBackground();
}

World::~World()
{
	for (const auto& [val, chunk] : m_Chunks)
		delete chunk;
	delete m_BorderAir;
	delete worldGenerator;
	for (Entity* entity : m_Entities)
		delete entity;
	delete m_Background;
}

void World::worldInit()
{
	m_BorderAir = new Tile({ 0, 0 }, g_TileManager.getTileRef("air"));
	worldGenerator = new Generator("GameData\\WorldGeneration\\Generator.lua");
	worldTileWidth = worldWidth * chunkWidth;

	//queueStructure(GenerateStructureStruct{ "Box", 20, 20 });
}

void World::recreateBackground()
{
	delete m_Background;
	m_Background = new StarBackground();
}

void World::update(lost::Bound2D renderBounds)
{
	//for (int x = floor(renderBounds.left / (chunkWidth * 32.0f)) - 1; x < ceil(renderBounds.right / (chunkWidth * 32.0f)) + 1; x++)
	//{
	//	if (m_Chunks.count(x))
	//}

	m_Background->update();


	// Update all chunks
	int newChunkGeneratingCount = 0;
	for (const auto& [val, chunk] : m_Chunks)
	{
		chunk->update();
		if (!chunk->ready)
			newChunkGeneratingCount++;
	}

	if (newChunkGeneratingCount != m_GeneratingChunks)
	{
		if (m_GeneratingStruct)
		{
			// If currently generating a structure update the generating chunks count on the conditional
			{
				std::lock_guard lock(m_StructureThreadMutex);
				m_GeneratingChunks = newChunkGeneratingCount;
			}
			m_StructureThreadCondition.notify_all();
		}
		else
		{
			m_GeneratingChunks = newChunkGeneratingCount;
		}
	}

	// Loop through all entities in the world and remove them from the world if necessary
	for (int i = m_Entities.size() - 1; i >= 0; i--)
	{
		m_Entities[i]->loopPosition(worldTileWidth * 32.0f);
		m_Entities[i]->update();
		if (m_Entities[i]->killEntity)
		{
			delete m_Entities[i];
			m_Entities.erase(m_Entities.begin() + i);
		}
	}

	// Remove any unused power circuits
	std::vector<int> eraseList = {};
	for (auto& [key, val] : m_PowerCircuits)
	{
		val.calcConsumption();
		if (val.connectedEntities.size() == 0)
			eraseList.push_back(key);
	}

	if (eraseList.size() > 0)
	{
		for (int& id : eraseList)
		{
			m_PowerCircuits.erase(id);
		}
	}

	// Check if generation thread needs to be joined
	if (!m_GeneratingStruct && m_ThreadActive && m_StructureGenerationThread.joinable())
	{
		m_ThreadActive = false;
		m_StructureGenerationThread.join();
	}

	// Check if generation thread needs to be started
	if (!m_GeneratingStruct && !m_StructureGenerationQueue.empty())
	{
		startGenerate();
	}
}

void World::renderBackground(lost::Bound2D renderBounds)
{
	m_Background->render();
}

void World::render(lost::Bound2D renderBounds)
{
	// Render Tiles
	for (int x = floor(renderBounds.left / (chunkWidth * 32.0f)) - 1; x < ceil(renderBounds.right / (chunkWidth * 32.0f)) + 1; x++)
	{
		if (x >= 0 && x < worldWidth)
		{
			if (m_Chunks.count(x))
			{
				if (m_Chunks[x]->ready)
				{
					m_Chunks[x]->renderTiles(renderBounds);
				}
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
				if (m_Chunks[loopedX]->ready)
				{
					m_Chunks[loopedX]->renderTilesAt(renderBounds, x);
				}
			}
			else
			{
				createChunk(loopedX);
			}
		}
	}

	// Render Tile Entities
	for (int layer = 0; layer < 3; layer++)
	{
		for (int x = floor(renderBounds.left / (chunkWidth * 32.0f)) - 1; x < ceil(renderBounds.right / (chunkWidth * 32.0f)) + 1; x++)
		{
			if (x >= 0 && x < worldWidth)
			{
				if (m_Chunks.count(x))
				{
					if (m_Chunks[x]->ready)
					{
						m_Chunks[x]->renderTileEntities(renderBounds, layer);
					}
				}
			}
			else
			{
				int loopedX = (x + worldWidth) % worldWidth;
				if (m_Chunks.count(loopedX))
				{
					if (m_Chunks[loopedX]->ready)
					{
						m_Chunks[loopedX]->renderTileEntitiesAt(renderBounds, layer, x);
					}
				}
			}
		}
	}

	for (Entity* entity : m_Entities)
		entity->render(renderBounds, worldTileWidth * 32.0f);
	//for (const auto& [val, chunk] : m_Chunks)
	//	chunk->renderBorders();

	ImGui::Text("Current Vertex Count: %i", m_Chunks.size() * chunkWidth * chunkHeight * 4);
}

void World::createChunk(int x)
{
	m_Chunks[x] = new Chunk({ x, 0 }, chunkWidth, chunkHeight);

	m_Chunks[x]->generateChunk(worldGenerator, this);
}

void World::finishCreation(int x)
{
	checkOutOfBoundEntities();
}

void World::checkOutOfBoundEntities()
{
	for (int i = m_OutOfBoundsEntities.size() - 1; i >= 0; i--)
	{
		OutOfBoundsTileEntityStruct& structVal = m_OutOfBoundsEntities[i];
		if (!checkPlaceOutOfBounds({ structVal.placeX, structVal.placeY, structVal.object->width, structVal.object->height }))
		{
			createTileEntity(structVal.object, structVal.x, structVal.y);
			m_OutOfBoundsEntities.erase(m_OutOfBoundsEntities.begin() + i);
		}
	}
}

void World::createTileEntity(TileEntityStruct* tileEntity, float x, float y)
{
	bool& requiresSupport = tileEntity->requiresSupport;
	lost::Vector2D placeLocation = { x + tileEntity->placementOffsetX, y + tileEntity->placementOffsetY };
	if (!checkPlaceOutOfBounds({ placeLocation.x, placeLocation.y, tileEntity->width, tileEntity->height }))
	{
		bool CanPlace = checkCanPlace({ placeLocation.x, placeLocation.y, tileEntity->width, tileEntity->height }, tileEntity->fillsLayers);
		if (requiresSupport && CanPlace)
			CanPlace = CanPlace && checkStable({ x + tileEntity->placementOffsetX, fmaxf(y + 1, placeLocation.y + tileEntity->height), tileEntity->width, 0.1 });

		if (CanPlace)
			addTileEntity(new TileEntity(tileEntity), placeLocation.x, placeLocation.y);
	}
	else
	{
		if (checkCanPlaceInBounds({ placeLocation.x, placeLocation.y, tileEntity->width, tileEntity->height }, tileEntity->fillsLayers))
			m_OutOfBoundsEntities.push_back(OutOfBoundsTileEntityStruct{ tileEntity, x, y, placeLocation.x, placeLocation.y });
	}
}

void World::addTileEntity(TileEntity* tileEntity, float x, float y)
{
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

	tileEntity->init();
}

void World::destroyTileEntity(TileEntity* tileEntity)
{
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

bool World::checkCanPlaceInBounds(lost::Bound2D bounds, std::array<bool, 3> layers)
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
		}
	}

	return !taken;
}

bool World::checkPlaceOutOfBounds(lost::Bound2D bounds)
{
	bool outOfBounds = false;

	// Loops through all covered tiles
	for (int y = floor(bounds.top); y < ceil(bounds.bottom) && !outOfBounds; y++)
	{
		for (int x = floor(bounds.left); x < ceil(bounds.right) && !outOfBounds; x++)
		{

			// Check if any of that tiles filledLayers match up with the layers given
			Tile* tile = getTileAt(x, y);
			if (tile == m_BorderAir)
			{
				outOfBounds = true;
			}

		}
	}

	return outOfBounds;
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
				if (!tile->getStable())
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
		if (m_Chunks[(int)floor(loopedX / (float)chunkWidth)]->ready)
		{
			m_Chunks[(int)floor(loopedX / (float)chunkWidth)]->setTile(tile, loopedX, y);
			updateTileNeighbors(loopedX, y);
		}
	}
}

void World::forceSetTile(TileRefStruct* tile, int x, int y)
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
	Tile* thisTile = forceGetTileAt(x, y);
	if (thisTile != m_BorderAir)
	{

		if (thisTile->referenceStruct->connectionType != "none")
		{

			std::array<int, 9> currentMatrix{ 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			for (int xOff = -1; xOff <= 1; xOff++)
			{
				for (int yOff = -1; yOff <= 1; yOff++)
				{
					Tile* offsetTile = forceGetTileAt(x + xOff, y + yOff);
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

void World::tileUpdate(int x, int y)
{
	for (int xOff = -1; xOff <= 1; xOff++)
	{
		for (int yOff = -1; yOff <= 1; yOff++)
		{
			getTileAt(x + xOff, y + yOff)->tileUpdate();
		}
	}
}

void World::tileUpdate(lost::IntVector2D pos)
{
	tileUpdate(pos.x, pos.y);
}

void World::tileUpdateArea(int x_, int y_, int w_, int h_)
{
	for (int x = x_; x < x_ + w_; x++)
	{
		for (int y = y_; y < y_ + h_; y++)
		{
			getTileAt(x, y)->tileUpdate();
		}
	}
}

void World::tileUpdateArea(lost::Bound2D area)
{
	tileUpdateArea((int)(area.x - 1.0f), (int)(area.y - 1.0f), (int)(area.w + 2.0f), (int)(area.h + 2.0f));
}

void World::addEntity(Entity* entity)
{
	m_Entities.push_back(entity);
}

Tile* World::getTileAt(int x, int y)
{
	if (m_Chunks.count((int)floor(loopX(x) / (float)chunkWidth)) > 0 && y >= 0 && y < chunkHeight)
	{
		if (m_Chunks[(int)floor(loopX(x) / (float)chunkWidth)]->ready)
			return m_Chunks[(int)floor(loopX(x) / (float)chunkWidth)]->getTile(loopX(x), y);
		return m_BorderAir;
	}
	return m_BorderAir;
}

Tile* World::forceGetTileAt(int x, int y)
{
	if (m_Chunks.count((int)floor(loopX(x) / (float)chunkWidth)) > 0 && y >= 0 && y < chunkHeight)
		return m_Chunks[(int)floor(loopX(x) / (float)chunkWidth)]->getTile(loopX(x), y);
	return m_BorderAir;
}

std::vector<TileEntity*> World::getTileEntitiesInArea(int x, int y, float range)
{
	std::vector<TileEntity*> inRange;

	int minXChunk = (int)floor(loopX(x - range) / (float)chunkWidth);
	int maxXChunk = (int)floor(loopX(x + range) / (float)chunkWidth);

	for (int x = minXChunk; x <= maxXChunk; x++)
	{
		std::vector<TileEntity*>& chunkEntities = m_Chunks[x]->getTileEntities();
		for (TileEntity* entity : chunkEntities)
		{
			if (entity->position.dist({ x * 32.0f, y * 32.0f }) / 32.0f <= range)
				inRange.push_back(entity);
		}
	}

	return inRange;
}

std::vector<TileEntity*> World::getTileEntitiesInBoxArea(int x, int y, float range)
{
	std::vector<TileEntity*> inRange;

	int minXChunk = (int)floor((x - range) / (float)chunkWidth);
	int maxXChunk = (int)floor((x + range) / (float)chunkWidth);

	lost::Bound2D areaBounds = { x - range, y - range, range * 2.0f + 1, range * 2.0f + 1 };

	for (int x = minXChunk; x <= maxXChunk; x++)
	{
		int chunkX = loopChunkX(x);

		std::vector<TileEntity*>& chunkEntities = m_Chunks[chunkX]->getTileEntities();
		for (TileEntity* entity : chunkEntities)
		{
			if (areaBounds.inBounds(entity->position))
				inRange.push_back(entity);
			else if (areaBounds.inBounds({ entity->position.x + worldTileWidth, entity->position.y }))
				inRange.push_back(entity);
			else if (areaBounds.inBounds({ entity->position.x - worldTileWidth, entity->position.y }))
				inRange.push_back(entity);
		}
	}

	return inRange;
}

int World::loopX(int x)
{
	return (x + worldTileWidth) % worldTileWidth;
}

float World::loopX(float x)
{
	return fmodf(x + worldTileWidth, worldTileWidth);
}

int World::loopChunkX(int x)
{
	return (x + worldWidth) % worldWidth;
}

float World::loopChunkX(float x)
{
	return fmodf(x + worldWidth, worldWidth);
}

uint32_t World::createPowerCircuit()
{
	PowerCircuitStruct circuit;
	
	size_t freeIndex = 0;
	while (freeIndex < SIZE_MAX)
	{
		if (m_PowerCircuits.count(freeIndex) == 0)
			break;
		freeIndex++;
	}

	circuit.circuitID = freeIndex;
	m_PowerCircuits[circuit.circuitID] = circuit;
	return circuit.circuitID;
}

PowerCircuitStruct& World::getPowerCircuit(uint32_t id)
{
	return m_PowerCircuits[id];
}

void World::mergePowerCircuits(uint32_t mergeOnto, uint32_t mergeFrom)
{
	PowerCircuitStruct& from = m_PowerCircuits[mergeFrom];
	std::map<uint32_t, uint32_t> fromIntMap = {};

	PowerCircuitStruct& onto = m_PowerCircuits[mergeOnto];

	// Precalculate the nodeIDs that nodes in from will have
	std::vector<uint32_t> nodeIDsToFill = onto.nextAvailableNodeIDs(from.nodeGraph.adjacencyMap.size());
	int count = 0;
	for (auto& [key, val] : from.nodeGraph.adjacencyMap)
	{
		fromIntMap[key] = nodeIDsToFill[count];
		count++;
	}

	for (TileEntity* entity : from.connectedEntities)
	{
		entity->setPowerCircuit(onto.circuitID);
		if (entity->tileType == "conduit")
		{
			PowerConduit* conduit = dynamic_cast<PowerConduit*>(entity);
			conduit->setTileCircuitReferences();

			// Set new node IDs to the nodes connected
			std::vector<uint32_t>& connectedNodes = conduit->getNodeConnections();
			conduit->setNode(fromIntMap[conduit->getNodeID()]);

			for (int i = 0; i < connectedNodes.size(); i++)
			{
				if (fromIntMap.count(connectedNodes[i]))
					connectedNodes[i] = fromIntMap[connectedNodes[i]];
			}

			if (connectedNodes.size() > 0)
				onto.addNodeByConnections(conduit->getNodeID(), conduit);
			else
				onto.addNode(conduit);
			// Node references inside the nodes must be done after all of them have been updated
		}
		/*else
		{
			onto.join(entity);
		}*/
	}

	from.connectedEntities = {};
}

void World::queueStructure(GenerateStructureStruct structureData)
{
	m_StructureGenerationQueue.push(structureData);
	if (!m_GeneratingStruct)
		startGenerate();
}

void World::startGenerate()
{
	std::cout << " [World::StartGenerate()] Started Generation of StructureQueue" << std::endl;
	if (!m_GeneratingStruct)
		m_StructureGenerationThread = std::thread(&World::generateStructure, this);
}

void World::generateStructure()
{
	// Mark the generation thread as active, preventing others from activating
	m_GeneratingStruct = true;
	m_ThreadActive = true;

	// Store the current amount of data on the queue
	// Using this to check if we've gone through the queue rather than checking the active length of it
	// removes the chance that we try and read the data of a generation structure struct while it's being
	// added by another thread
	int loopCount = m_StructureGenerationQueue.size();

	for (int i = 0; i < loopCount; i++)
	{
		// Get the first index's data from the queue
		GenerateStructureStruct structureData = m_StructureGenerationQueue.front();

		std::cout << " [World::GenerateStructure()] Started Generation of " << structureData.structureID << std::endl;

		// Create the lua state
		lua_State* L = luaL_newstate();
		luaL_openlibs(L);
		luaBindDebugPrint(L);

		// Bind c functions
		lua_pushcfunction(L, LuaAwaitChunkGeneration);
		lua_setglobal(L, "AwaitChunkGeneration");
		lua_pushcfunction(L, LuaSetTileAt);
		lua_setglobal(L, "SetTile");
		lua_pushcfunction(L, LuaGetTileIDAt);
		lua_setglobal(L, "GetTile");

		if (structureData.extraData)
		{
			JSONObjectToLuaTable(structureData.extraData, L, "extraData");
			delete structureData.extraData;
		}

		std::string preSetData = "spawnPos = { x = " + std::to_string(structureData.startPos.x) + ", y = " + std::to_string(structureData.startPos.y) + " }";

		checkLua(L, luaL_dostring(L, preSetData.c_str()));

		// Run the lua code
		if (m_StructureGeneratorCode.count(structureData.structureID))
			checkLua(L, luaL_dostring(L, m_StructureGeneratorCode[structureData.structureID].c_str()));
		else
			lost::lassert("Structure with ID: " + structureData.structureID + " doesn't exist! Maybe file name is different to ID in generator?");

		lua_close(L);

		// Remove the first index from the queue
		m_StructureGenerationQueue.pop();
	}

	// Unmark the generation thread
	m_GeneratingStruct = false;
}

void World::awaitChunkGeneration(int minX, int maxX)
{
	// Convert from tile units to chunk units
	minX = floor((float)minX / chunkWidth);
	maxX = floor((float)maxX / chunkWidth);

	int chunkCount = maxX - minX + 1;
	bool* chunkComplete = new bool[chunkCount];
	for (int i = 0; i < chunkCount; i++)
		chunkComplete[i] = false;
	
	// Generate all the chunks required
	for (int x = minX; x <= maxX; x++)
	{
		std::cout << " [World::AwaitChunkGeneration()] Started Generating chunk " << loopChunkX(x) << std::endl;

		if (m_Chunks.count(loopChunkX(x)))
		{
			// Chunk already generated
			chunkComplete[x - minX] = true;

			// [=] The edge case where the chunk is still being generated, shouldn't occur anymore, as it awaits for ALL chunks to be generated
		}
		else
		{
			// Chunk not generated, create it
			m_Chunks[loopChunkX(x)] = new Chunk({ loopChunkX(x), 0}, chunkWidth, chunkHeight);
			m_Chunks[loopChunkX(x)]->generateChunkMutex(worldGenerator, this, m_StructureThreadMutex, m_StructureThreadCondition, chunkComplete + (x - minX));
		}
	}

	std::cout << " [World::AwaitChunkGeneration()] Awaiting Completion" << std::endl;

	// Lock this thread until all chunks are complete
	int* currentGeneratingChunks = &m_GeneratingChunks; // Need to create a reference in this function for it to be captured

	std::unique_lock lock(m_StructureThreadMutex);
	m_StructureThreadCondition.wait(lock, [chunkComplete, chunkCount, currentGeneratingChunks]{
		for (int i = 0; i < chunkCount; i++)
		{
			if (!chunkComplete[i])
				return false;
		}
		return *currentGeneratingChunks == 0;
	});

	std::cout << " [World::AwaitChunkGeneration()] Completed" << std::endl;

	delete[] chunkComplete;
}

void World::addStructureCode(std::string ID, std::string code)
{
	m_StructureGeneratorCode[ID] = code;
}

void World::addDroneBay(DroneBay* bay)
{
	m_WorldDroneBays[bay->getName()].push_back(bay);
}

void World::removeDroneBay(DroneBay* bay)
{
	std::vector<DroneBay*>& nameGroup = m_WorldDroneBays[bay->getName()];
	for (int i = 0; i < nameGroup.size(); i++)
	{
		if (nameGroup[i] == bay)
		{
			nameGroup.erase(nameGroup.begin() + i);
			break;
		}
	}
}

DroneBay* World::getTransferBay(DroneBay* thisBay)
{
	std::vector<DroneBay*>& nameGroup = m_WorldDroneBays[thisBay->getName()];

	DroneBay* lowestBay = nullptr;

	for (int i = 0; i < nameGroup.size(); i++)
	{
		//if (nameGroup[i].canTransfer(thisBay->))
		if (nameGroup[i] != thisBay && !nameGroup[i]->isSending())
		{
			if (lowestBay)
			{
				if (lowestBay->canRecieve(*nameGroup[i]->getInventory()->getItem(0)))
				{
					if (lowestBay->getInventory()->getItem(0)->StackSize > nameGroup[i]->getInventory()->getItem(0)->StackSize)
						lowestBay = nameGroup[i];
				}
			}
			else
			{
				lowestBay = nameGroup[i];
				if (!lowestBay->canRecieve(*nameGroup[i]->getInventory()->getItem(0)))
					lowestBay = nullptr;
			}
		}
	}

	return lowestBay;
}

World* g_World = nullptr;

void PowerCircuitStruct::removeNode(PowerConduit* conduit)
{
	std::vector<Graph> split = nodeGraph.removeNode(conduit->getNodeID());

	// Remove the conduit that called this function
	for (int i = 0; i < connectedEntities.size(); i++)
	{
		if (connectedEntities[i] == conduit) // Check if the locations in memory are the same
		{
			connectedEntities.erase(connectedEntities.begin() + i);
			break;
		}
	}

	if (split.size() > 0)
	{
		// Turn the splits into more circuits, and update the conduits references
		for (int i = 0; i < split.size(); i++)
		{
			// Create a new circuit
			uint32_t splitCircuitID = g_World->createPowerCircuit();
			PowerCircuitStruct& splitCircuit = g_World->getPowerCircuit(splitCircuitID);

			// Set the node graph of that split circuit to the given split graph
			splitCircuit.nodeGraph = split[i];
			std::vector<PowerConduit*> toUpdateConduits;

			// Loop over all connected entities in this graph and change their references to the other circuit
			for (int j = connectedEntities.size() - 1; j >= 0; j--)
			{
				TileEntity* entity = connectedEntities[j];
				if (entity->tileType == "conduit")
				{
					PowerConduit* selectedConduit = dynamic_cast<PowerConduit*>(entity);

					// .hasNode() only works in this case since every ID is unique across every split graph returned
					if (splitCircuit.nodeGraph.hasNode(selectedConduit->getNodeID()))
					{
						selectedConduit->setPowerCircuit(splitCircuitID);
						toUpdateConduits.push_back(selectedConduit);
						// nodeID doesn't need to be set since the graphs keep the original nodeIDs from the unsplit graph

						// Move connected entity reference from this circuit to the split one
						splitCircuit.connectedEntities.push_back(entity);
						connectedEntities.erase(connectedEntities.begin() + j);
					}
				}
			}

			for (PowerConduit* selectedConduit : toUpdateConduits)
			{
				selectedConduit->setTileCircuitReferences();
			}
		}
	}
};

int LuaAwaitChunkGeneration(lua_State* L)
{
	int minX = lua_tointeger(L, 1);
	int maxX = lua_tointeger(L, 2);
	lua_pop(L, -1);
	lua_pop(L, -1);

	g_World->awaitChunkGeneration(minX, maxX);
	return 0;
}

int LuaGetTileIDAt(lua_State* L)
{
	int x = lua_tointeger(L, 1);
	int y = lua_tointeger(L, 2);
	lua_pop(L, -1);
	lua_pop(L, -1);
	lua_pushstring(L, g_World->forceGetTileAt(x, y)->referenceStruct->name.c_str());
	return 1;
}

int LuaSetTileAt(lua_State* L)
{
	int x = lua_tointeger(L, 2);
	int y = lua_tointeger(L, 3);
	std::string id = lua_tostring(L, 1);
	lua_pop(L, -1);
	lua_pop(L, -1);
	g_World->forceSetTile(g_TileManager.getTileRef(id), x, y);
	return 0;
}
