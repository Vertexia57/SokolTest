#include "ConveyerBeltTileEntity.h"

ConveyerBeltTileEntity::ConveyerBeltTileEntity(TileEntityStruct* tileEntityRef_)
	: TileEntity(tileEntityRef_)
{
	updates = true;
	relativeVelocity = { tileEntityRef->updateData->getFloat("xSpeed"), 0.0f };
}

ConveyerBeltTileEntity::~ConveyerBeltTileEntity()
{
}

void ConveyerBeltTileEntity::update()
{

}
