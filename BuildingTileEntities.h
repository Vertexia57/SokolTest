#pragma once
#include "World.h"
#include "ChestTileEntity.h"
#include "ConveyerBeltTileEntity.h"
#include "OreDrill.h"
#include "PowerConduit.h"
#include "DistributorTileEntity.h"
#include "JunctionTileEntity.h"
#include "FactoryTileEntity.h"
#include "DroneBay.h"

static std::map<std::string, int> updateActionNames = {
	{ "none", 0 },
	{ "chest", 1 },
	{ "miner", 2 },
	{ "powerConduit", 3 },
	{ "conveyerBelt", 4 },
	{ "distributor", 5 },
	{ "junction", 6 },
	{ "factory", 7 },
	{ "droneBay", 8 }
};

static void createBuilding(TileEntityStruct* tileEntityRef, lost::Vector2D position, uint32_t rotation)
{
	if (tileEntityRef->building)
	{
		int buildingType = updateActionNames[tileEntityRef->updateAction];

		float buildX = floor(position.x + tileEntityRef->placementOffsetX);
		float buildY = floor(position.y + tileEntityRef->placementOffsetY);

		switch (buildingType)
		{
		case 0: // none
			g_World->addTileEntity(new TileEntity(tileEntityRef, rotation), buildX, buildY);
			break;
		case 1: // chest
			g_World->addTileEntity(new ChestTileEntity(tileEntityRef, rotation), buildX, buildY);
			break;
		case 2: // miner
			g_World->addTileEntity(new OreDrillEntity(tileEntityRef, rotation), buildX, buildY);
			break;
		case 3: // powerConduit
			g_World->addTileEntity(new PowerConduit(tileEntityRef, rotation), buildX, buildY);
			break;
		case 4: // conveyerBelt
			g_World->addTileEntity(new ConveyerBeltTileEntity(tileEntityRef, rotation), buildX, buildY);
			break;
		case 5: // distributor
			g_World->addTileEntity(new DistributorTileEntity(tileEntityRef, rotation), buildX, buildY);
			break;
		case 6: // junction
			g_World->addTileEntity(new JunctionTileEntity(tileEntityRef, rotation), buildX, buildY);
			break;
		case 7: // factory
			g_World->addTileEntity(new FactoryTileEntity(tileEntityRef, rotation), buildX, buildY);
			break;
		case 8: // droneBay
			g_World->addTileEntity(new DroneBay(tileEntityRef, rotation), buildX, buildY);
			break;
		default:
			g_World->addTileEntity(new TileEntity(tileEntityRef, rotation), buildX, buildY);
			break;
		}
	}
	else
	{
		std::cout << "Tried to build non building tile entity\n";
	}
}