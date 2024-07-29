#pragma once
#include "ConveyerBeltTileEntity.h"

class JunctionTileEntity : public ConveyerBeltTileEntity
{
public:
	JunctionTileEntity(TileEntityStruct* tileEntityRef_, uint32_t rotation = 0);
	virtual ~JunctionTileEntity() override;

	virtual void update() override;
	virtual void tileUpdate() override;

	virtual void renderForeground() override;
	virtual void renderForegroundAt(lost::Vector2D pos) override;

	virtual bool getEmpty(int direction) const;

	virtual void checkNeighbors();
	virtual void passItem(ConveyerBeltTileEntity* other, lost::IntVector2D tileOffset, int directionFrom, ConveyerBeltItem*& item);
	virtual void recieveItem(ConveyerBeltItem* item, lost::IntVector2D tileOffset, int directionFrom);

protected:
	float m_Cooldown = 0.0f;
	float m_TransferTime[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	ConveyerBeltItem* m_DirectionHeldItems[4] = { nullptr, nullptr, nullptr, nullptr };
};

