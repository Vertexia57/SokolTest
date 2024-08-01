#pragma once
#include "Tile.h"
#include "TileEntity.h"
#include "Chunk.h"
#include <array>
#include "Generator.h"
#include "Entity.h"
#include "Lost/Transform2D.h"
#include <map>
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
	void updateTileConnections(int x, int y);
	void updateTileNeighbors(int x, int y);

	void tileUpdate(int x, int y);
	void tileUpdate(lost::IntVector2D pos);
	void tileUpdateArea(int x, int y, int w, int h);
	void tileUpdateArea(lost::Bound2D area);

	void addEntity(Entity* entity);

	Tile* getTileAt(int x, int y);
	Tile* forceGetTileAt(int x, int y);

	std::vector<TileEntity*> getTileEntitiesInArea(int x, int y, float range);
	std::vector<TileEntity*> getTileEntitiesInBoxArea(int x, int y, float range);

	int loopX(int x);
	float loopX(float x);
	int loopChunkX(int x);
	float loopChunkX(float x);

	uint32_t createPowerCircuit();
	PowerCircuitStruct& getPowerCircuit(uint32_t id);
	void mergePowerCircuits(uint32_t mergeOnto, uint32_t mergeFrom);

	int worldMaxX = 0;
	int worldMinX = 0;

	int chunkWidth = 32;
	int chunkHeight = 96;

	int worldTileWidth;
	int worldWidth = 20;
private:
	Tile* m_BorderAir = nullptr;

	std::map<int, Chunk*> m_Chunks;
	std::vector<Entity*> m_Entities;

	std::vector<OutOfBoundsTileEntityStruct> m_OutOfBoundsEntities;

	std::map<int, PowerCircuitStruct> m_PowerCircuits;

	Generator* worldGenerator = nullptr;

	Background* m_Background = nullptr;
};