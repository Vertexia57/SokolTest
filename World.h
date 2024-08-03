#pragma once
#include <map>
#include <array>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

#include "Tile.h"
#include "TileEntity.h"
#include "Chunk.h"
#include "Generator.h"
#include "Entity.h"
#include "Lost/Transform2D.h"
#include "Lost/Graph.h"
#include "PowerConduit.h"
#include "StarBackground.h"

class World;
extern World* g_World; // Global World Singleton

struct OutOfBoundsTileEntityStruct
{
	TileEntityStruct* object;
	float x;
	float y;
	float placeX;
	float placeY;
};

struct PowerCircuitStruct
{
	uint32_t circuitID = 0;
	float totalConsumption = 0.0f;
	float totalProduce = 0.0f;
	float satisfaction = 1.0f;
	std::vector<TileEntity*> connectedEntities = {};
	Graph nodeGraph = {};

	float calcConsumption()
	{
		totalConsumption = 0.0f;
		totalProduce = 0.0f;
		for (TileEntity* e : connectedEntities)
		{
			totalConsumption += e->getPowerConsumption();
			totalProduce += e->getPowerProduce();
		}

		if (totalConsumption > 0.0f)
			satisfaction = fminf(totalProduce / totalConsumption, 1.0f);
		else
			satisfaction = 0.0f;

		if (ImGui::BeginChild(("Circuit" + std::to_string(circuitID)).c_str(), ImVec2(0, 0), ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeY, ImGuiWindowFlags_MenuBar))
		{
			if (ImGui::BeginMenuBar())
			{
				ImGui::Text(("Electrical Circuit " + std::to_string(circuitID)).c_str());
				ImGui::EndMenuBar();
			}
			ImGui::Text("Satisfaction: %f", satisfaction);
			ImGui::Text("Consumption: %f", totalConsumption);
			ImGui::Text("Production: %f", totalProduce);
			ImGui::Text("Overall: %f, %f/%f", satisfaction, totalProduce, totalConsumption);
			ImGui::Spacing();
			ImGui::Text("Connected Entities: %i", connectedEntities.size());
			ImGui::EndChild();
		}

		return totalConsumption;
	};

	uint32_t addNode(PowerConduit* conduit)
	{
		connectedEntities.push_back(conduit);
		return nodeGraph.addNode(conduit->getNodeConnections());
	};

	uint32_t addNodeByConnections(uint32_t nodeID, PowerConduit* conduit)
	{
		connectedEntities.push_back(conduit);
		return nodeGraph.addNodeByConnections(nodeID, conduit->getNodeConnections());
	};

	// Defined in cpp
	void removeNode(PowerConduit* conduit);

	uint32_t nextAvailableNodeID() const
	{
		return nodeGraph.nextAvailableNodeID();
	}

	std::vector<uint32_t> nextAvailableNodeIDs(int count) const
	{
		return nodeGraph.nextAvailableNodeIDs(count);
	}

	void join(TileEntity* entity)
	{
		connectedEntities.push_back(entity);
	};

	void leave(TileEntity* entity)
	{
		for (int i = 0; i < connectedEntities.size(); i++)
		{
			if (connectedEntities[i] == entity)
			{
				connectedEntities.erase(connectedEntities.begin() + i);
				break;
			}
		}
	}
};

struct GenerateStructureStruct
{
	std::string structureID;
	lost::IntVector2D startPos;
	lost::Bound2D bounds = { 0.0f, 0.0f, 0.0f, 0.0f };
};

class World
{
public:
	World();
	~World();

	void worldInit();
	void recreateBackground();

	void update(lost::Bound2D renderBounds);
	void renderBackground(lost::Bound2D renderBounds);
	void render(lost::Bound2D renderBounds);

	void createChunk(int x);
	void finishCreation(int x);
	void checkOutOfBoundEntities();

	void createTileEntity(TileEntityStruct* tileEntity, float x, float y);
	void addTileEntity(TileEntity* tileEntity, float x, float y);
	void destroyTileEntity(TileEntity* tileEntity);

	// Checks if the area covered by bounds has any tiles already taking the layers specified
	bool checkCanPlace(lost::Bound2D bounds, std::array<bool, 3> layers);
	// Checks if the area covered by bounds has any tiles already taking the layers specified
	bool checkCanPlaceInBounds(lost::Bound2D bounds, std::array<bool, 3> layers);
	// Checks if the area covered by bounds has any tiles already taking the layers specified
	bool checkPlaceOutOfBounds(lost::Bound2D bounds);
	// Checks if the area covered by bounds are all stable
	bool checkStable(lost::Bound2D bounds);

	void setTile(TileRefStruct* tile, int x, int y);
	// Ignores the chunks "ready" flag
	void forceSetTile(TileRefStruct* tile, int x, int y);

	// [--------------------]
	//    Update Functions
	// [--------------------]

	// Updates the tile's coneection in and around the tile location given
	void updateTileConnections(int x, int y);
	// Updates the tile's coneection in and around the tile location given
	void updateTileNeighbors(int x, int y);

	// Updates an individual tile
	void tileUpdate(int x, int y);
	// Updates an individual tile
	void tileUpdate(lost::IntVector2D pos);
	// Updates the tiles in the area given
	void tileUpdateArea(int x, int y, int w, int h);
	// Updates the tiles in the area given
	void tileUpdateArea(lost::Bound2D area);



	void addEntity(Entity* entity);

	// [----------------------]
	//    Get Tile Functions
	// [----------------------]

	Tile* getTileAt(int x, int y);
	// Ignores the chunks "ready" flag
	Tile* forceGetTileAt(int x, int y);

	// Returns the tile entities in the area given, uses distance to check if it's in range
	std::vector<TileEntity*> getTileEntitiesInArea(int x, int y, float range);
	// Returns the tile entities in the area given, uses a box to check if it's in range
	std::vector<TileEntity*> getTileEntitiesInBoxArea(int x, int y, float range);

	// [---------------------]
	//    Looping Functions
	// [---------------------]
	
	// Loops the x value around the world width, uses tile units
	int loopX(int x);
	// Loops the x value around the world width, uses tile units
	float loopX(float x);
	// Loops the x value around the world width, uses chunk units
	int loopChunkX(int x);
	// Loops the x value around the world width, uses chunk units
	float loopChunkX(float x);

	// [---------------------------]
	//    Power Network Functions
	// [---------------------------]

	// Creates a power circuit and returns the id of that circuit
	uint32_t createPowerCircuit();
	// Returns the power circuit struct with the id given
	PowerCircuitStruct& getPowerCircuit(uint32_t id);
	// Merges two power networks into one
	void mergePowerCircuits(uint32_t mergeOnto, uint32_t mergeFrom);

	// [-----------------------]
	//    Structure Functions
	// [-----------------------]

	// Adds a structure to the queue
	void queueStructure(GenerateStructureStruct structureData);
	// Starts the next generation, is ran if no structure is being generated / the queue is empty
	// Note: Is only ran when after a chunk is generated
	void startGenerate();
	// Generates the structure that is first in the queue, this is the one that runs on the structure thread
	void generateStructure();
	// A function ran by lua, used as a break point to generate the chunks
	void awaitChunkGeneration(int minX, int maxX);
	
	void addStructureCode(std::string ID, std::string code);

	int worldMaxX = 0;
	int worldMinX = 0;

	int chunkWidth = 32;
	int chunkHeight = 96;

	int worldTileWidth;
	int worldWidth = 20;
private:
	Tile* m_BorderAir = nullptr;

	std::mutex m_StructureThreadMutex;
	std::condition_variable m_StructureThreadCondition;
	bool m_ThreadActive = false;
	bool m_GeneratingStruct = false;
	int m_GeneratingChunks = 0;
	std::thread m_StructureGenerationThread;
	std::queue<GenerateStructureStruct> m_StructureGenerationQueue;

	std::map<int, Chunk*> m_Chunks;
	std::vector<Entity*> m_Entities;

	std::vector<OutOfBoundsTileEntityStruct> m_OutOfBoundsEntities;

	std::map<int, PowerCircuitStruct> m_PowerCircuits;

	Generator* worldGenerator = nullptr;
	std::map<std::string, std::string> m_StructureGeneratorCode = {};

	Background* m_Background = nullptr;
};

extern int LuaAwaitChunkGeneration(lua_State* L);
extern int LuaGetTileIDAt(lua_State* L);
extern int LuaSetTileAt(lua_State* L);