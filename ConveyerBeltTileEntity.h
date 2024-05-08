#pragma once
#include "TileEntity.h"
#include "Item.h"

// [!] TODO: Figure this out, maybe use a pen and paper
struct ConveyerBeltItem
{
	float x;
};

class ConveyerBeltTileEntity : public TileEntity
{
public:
	ConveyerBeltTileEntity(TileEntityStruct* tileEntityRef_);
	virtual ~ConveyerBeltTileEntity();

	virtual void update() override;

	// Uses base tile entities rendering
	//virtual void render() override;
	//virtual void renderAt(lost::Vector2D pos) override;

	void addItem(Item item);
protected:
};

