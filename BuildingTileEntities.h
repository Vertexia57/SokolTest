#pragma once
#include "World.h"
#include "ChestTileEntity.h"
#include "ConveyerBeltTileEntity.h"

static std::map<std::string, int> updateActionNames = {
	{ "none", 0 },
	{ "chest", 1 },
	{ "miner", 2 },
	{ "powerConduit", 3 },
	{ "conveyerBelt", 4 }
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
			g_World->addTileEntity(new TileEntity(tileEntityRef, rotation), buildX, buildY);
			break;
		case 3: // powerConduit
			g_World->addTileEntity(new TileEntity(tileEntityRef, rotation), buildX, buildY);
			break;
		case 4: // conveyerBelt
			g_World->addTileEntity(new ConveyerBeltTileEntity(tileEntityRef, rotation), buildX, buildY);
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