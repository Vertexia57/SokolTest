#include "PowerConduit.h"
#include "World.h"
#include "Lost/FontManager.h"

PowerConduit::PowerConduit(TileEntityStruct* tileEntityRef_, uint32_t rotation)
	: TileEntity(tileEntityRef_, rotation)
{
	updates = false;
	interactable = false;

	tileType = "conduit";

	powerRange = tileEntityRef->updateData->getInt("powerRange");
	connectionRange = tileEntityRef->updateData->getInt("range");

	if (powerRange > connectionRange)
		std::cout << " [ERROR] A Conduits power range was greater than it's connection range!\n";
	if (powerRange > g_World->worldTileWidth / 2 || connectionRange > g_World->worldTileWidth / 2)
		std::cout << " [ERROR] A Conduits power range or connection range was greater than half the world length!\n";
}

PowerConduit::~PowerConduit()
{
	g_World->getPowerCircuit(m_PowerCircuit).removeNode(this);

	for (PowerConduit* conduit : m_ConnectedConduits)
	{
		for (int i = 0; i < conduit->m_ConnectedConduits.size(); i++)
		{
			if (conduit->m_ConnectedConduits[i] == this)
			{
				conduit->m_ConnectedConduits.erase(conduit->m_ConnectedConduits.begin() + i);
				conduit->m_ConnectedNodes.erase(conduit->m_ConnectedNodes.begin() + i);
			}
		}
	}

	// Update Tile References
	lost::IntVector2D worldPos = { (int)(m_Hitbox.x + m_Hitbox.w / 2.0f), (int)(m_Hitbox.y + m_Hitbox.h / 2.0f) };
	for (int x = floor(worldPos.x - powerRange); x < ceil(worldPos.x + powerRange + 1); x++)
	{
		for (int y = floor(worldPos.y - powerRange); y < ceil(worldPos.y + powerRange + 1); y++)
		{
			Tile* tile = g_World->getTileAt(x, y);
			tile->leavePowerCircuit();
			tile->forceTileUpdate();
		}
	}
}

void PowerConduit::init()
{
	TileEntity::init();
	
	// Connect to any pre-existing conduits
	lost::IntVector2D worldPos = { (int)(m_Hitbox.x + m_Hitbox.w / 2.0f), (int)(m_Hitbox.y + m_Hitbox.h / 2.0f) };
	std::vector<TileEntity*> entitiesInRange = g_World->getTileEntitiesInBoxArea(worldPos.x, worldPos.y, connectionRange);

	PowerConduit* conduit = nullptr;

	for (TileEntity* entity : entitiesInRange)
	{
		if (entity->tileType == "conduit" && entity != this) // Check if conduit
		{
			int xOff = worldPos.x - (int)entity->position.x;
			int yOff = worldPos.y - (int)entity->position.y;

			if (worldPos.x - entity->position.x >= g_World->worldTileWidth / 2)
				xOff = worldPos.x - (int)entity->position.x - g_World->worldTileWidth;
			else if (worldPos.x - entity->position.x <= -g_World->worldTileWidth / 2)
				xOff = worldPos.x - (int)entity->position.x + g_World->worldTileWidth;

			PowerConduit* conduitEntity = dynamic_cast<PowerConduit*>(entity);
			int otherRange = conduitEntity->connectionRange;

			// Checks if self is in other conduit's range
			if (xOff < otherRange + 1 && xOff > -otherRange - 1 &&
				yOff < otherRange + 1 && yOff > -otherRange - 1)
			{
				// "conduit" is the main conduit it will connect to, any others will be merged on if they are apart of a seperate circuit
				if (!conduit)
					conduit = conduitEntity;

				if (conduitEntity->getPowerCircuit() != conduit->getPowerCircuit())
					g_World->mergePowerCircuits(conduit->getPowerCircuit(), conduitEntity->getPowerCircuit());

				// Add to connected conduits lists
				m_ConnectedConduits.push_back(conduitEntity);
				m_ConnectedNodes.push_back(conduitEntity->m_NodeGraphID);
			}
			
		}
	}

	if (conduit)
	{
		m_PowerCircuit = conduit->getPowerCircuit();

		// Add this node id to the other conduits
		for (PowerConduit* c : m_ConnectedConduits)
		{
			c->m_ConnectedNodes.push_back(g_World->getPowerCircuit(m_PowerCircuit).nextAvailableNodeID());
			c->m_ConnectedConduits.push_back(this);
		}

		m_NodeGraphID = g_World->getPowerCircuit(m_PowerCircuit).addNode(this);
	}
	else
	{
		m_PowerCircuit = g_World->createPowerCircuit();
		m_NodeGraphID = g_World->getPowerCircuit(m_PowerCircuit).addNode(this);
		// No nodes to connect to
	}

	// Update Tile References
	for (int x = floor(worldPos.x - powerRange); x < ceil(worldPos.x + powerRange + 1); x++)
	{
		for (int y = floor(worldPos.y - powerRange); y < ceil(worldPos.y + powerRange + 1); y++)
		{
			Tile* tile = g_World->getTileAt(x, y);
			tile->joinPowerCircuit(m_PowerCircuit);
			tile->forceTileUpdate();
		}
	}
}

void PowerConduit::tileUpdate()
{
	m_TileUpdated = true;

	// Check power sources
	//checkPowerCircuit();

	//lost::IntVector2D worldPos = { (int)(m_Hitbox.x + m_Hitbox.w / 2.0f), (int)(m_Hitbox.y + m_Hitbox.h / 2.0f) };
}

void PowerConduit::setNode(int nodeID)
{
	m_NodeGraphID = nodeID;
}

void PowerConduit::setTileCircuitReferences()
{
	// Update Tile References
	lost::IntVector2D worldPos = { (int)(m_Hitbox.x + m_Hitbox.w / 2.0f), (int)(m_Hitbox.y + m_Hitbox.h / 2.0f) };
	for (int x = floor(worldPos.x - powerRange); x < ceil(worldPos.x + powerRange + 1); x++)
	{
		for (int y = floor(worldPos.y - powerRange); y < ceil(worldPos.y + powerRange + 1); y++)
		{
			Tile* tile = g_World->getTileAt(x, y);
			tile->setPowerCircuit(m_PowerCircuit);
			tile->forceTileUpdate();
		}
	}
}

void PowerConduit::renderForeground()
{

	for (int i = 0; i < m_ConnectedConduits.size(); i++)
	{
		lost::clearImage();
		lost::unbindShader();
		if (m_ConnectedConduits[i]->position.x - position.x >= 100.0f)
			sgp_draw_line((float)position.x * 32.0f + 16.0f, (float)position.y * 32.0f + 16.0f, (float)(m_ConnectedConduits[i]->position.x - g_World->worldTileWidth) * 32.0f + 16.0f, (float)m_ConnectedConduits[i]->position.y * 32.0f + 16.0f);
		else if (m_ConnectedConduits[i]->position.x - position.x <= -100.0f)
			sgp_draw_line((float)position.x * 32.0f + 16.0f, (float)position.y * 32.0f + 16.0f, (float)(m_ConnectedConduits[i]->position.x + g_World->worldTileWidth) * 32.0f + 16.0f, (float)m_ConnectedConduits[i]->position.y * 32.0f + 16.0f);
		else
			sgp_draw_line((float)position.x * 32.0f + 16.0f, (float)position.y * 32.0f + 16.0f, (float)m_ConnectedConduits[i]->position.x * 32.0f + 16.0f, (float)m_ConnectedConduits[i]->position.y * 32.0f + 16.0f);

		//lost::renderTextPro(std::to_string(m_PowerCircuit), { position.x * 32.0f + 16.0f, position.y * 32.0f + 16.0f }, 1.0, LOST_TEXT_ALIGN_MIDDLE, LOST_TEXT_ALIGN_BOTTOM);
		//lost::renderTextPro(std::to_string(m_ConnectedNodes[i]), {position.x * 32.0f + 16.0f, position.y * 32.0f - 7.0f * i}, 0.2, LOST_TEXT_ALIGN_MIDDLE, LOST_TEXT_ALIGN_BOTTOM);
		lost::resetShader();
	}
}

void PowerConduit::renderForegroundAt(lost::Vector2D pos)
{
	for (int i = 0; i < m_ConnectedConduits.size(); i++)
	{
		lost::clearImage();
		lost::unbindShader();
		if (m_ConnectedConduits[i]->position.x - pos.x >= 100.0f)
			sgp_draw_line((float)pos.x * 32.0f + 16.0f, (float)pos.y * 32.0f + 16.0f, (float)(m_ConnectedConduits[i]->position.x - g_World->worldTileWidth) * 32.0f + 16.0f, (float)m_ConnectedConduits[i]->position.y * 32.0f + 16.0f);
		else if (m_ConnectedConduits[i]->position.x - pos.x <= -100.0f)
			sgp_draw_line((float)pos.x * 32.0f + 16.0f, (float)pos.y * 32.0f + 16.0f, (float)(m_ConnectedConduits[i]->position.x + g_World->worldTileWidth) * 32.0f + 16.0f, (float)m_ConnectedConduits[i]->position.y * 32.0f + 16.0f);
		else
			sgp_draw_line((float)pos.x * 32.0f + 16.0f, (float)pos.y * 32.0f + 16.0f, (float)m_ConnectedConduits[i]->position.x * 32.0f + 16.0f, (float)m_ConnectedConduits[i]->position.y * 32.0f + 16.0f);
		lost::resetShader();
	}
}
