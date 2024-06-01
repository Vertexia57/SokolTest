#pragma once
#include "TileEntity.h"
#include "Item.h"

// [!] TODO: Figure this out, maybe use a pen and paper
struct ConveyerBeltItem
{
	Item item;
	float x;
};

class ConveyerBeltTileEntity : public TileEntity
{
public:
	ConveyerBeltTileEntity(TileEntityStruct* tileEntityRef_);
	virtual ~ConveyerBeltTileEntity() override;

	virtual void init() override;

	virtual void update() override;

	virtual void render() override;
	virtual void renderAt(lost::Vector2D pos) override;

	void checkNeighbors();
	void addItem(Item item);
	void passItem(ConveyerBeltTileEntity* other, lost::IntVector2D tileOffset);

	inline bool getEmpty() const { return (heldItem == nullptr || m_Moving); };
protected:
	ConveyerBeltTileEntity* m_Left = nullptr;
	ConveyerBeltTileEntity* m_Right = nullptr;

	bool m_Moving = false;
	ConveyerBeltItem* heldItem = nullptr;
};

