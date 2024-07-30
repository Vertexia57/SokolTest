#pragma once
#include "TileEntity.h"
#include "Item.h"

struct ConveyerBeltItem
{
	Item item;
	float x;
	float y;
	int directionFrom;
};

class ConveyerBeltTileEntity : public TileEntity
{
public:
	ConveyerBeltTileEntity(TileEntityStruct* tileEntityRef_, uint32_t rotation = 0);
	virtual ~ConveyerBeltTileEntity() override;

	virtual void init() override;

	virtual void update() override;
	virtual void tileUpdate() override;

	virtual void render() override;
	virtual void renderAt(lost::Vector2D pos) override;
	virtual void renderForeground() override;
	virtual void renderForegroundAt(lost::Vector2D pos) override;

	virtual void checkNeighbors();

	virtual void insertItem(Item& item);
	virtual bool canInsert(Item& item) const;

	void checkPasses();
	virtual void passItem(ConveyerBeltTileEntity* other, lost::IntVector2D tileOffset, int directionFrom);
	virtual void recieveItem(ConveyerBeltItem* item, lost::IntVector2D tileOffset, int directionFrom);

	virtual bool getEmpty(int direction) const;
	inline uint32_t getRotation() const { return m_Rotation; };
	inline ConveyerBeltItem*& getHeldItem() { return heldItem; };
protected:
	ConveyerBeltTileEntity* m_Left = nullptr;
	ConveyerBeltTileEntity* m_Right = nullptr;
	ConveyerBeltTileEntity* m_Up = nullptr;
	ConveyerBeltTileEntity* m_Down = nullptr;

	TileEntity* m_LeftEntity = nullptr;
	TileEntity* m_RightEntity = nullptr;
	TileEntity* m_UpEntity = nullptr;
	TileEntity* m_DownEntity = nullptr;

	lost::Vector2D m_Speeds = { 0.0f, 0.0f };
	bool m_Diagonal = false;

	bool m_Moving = false;
	ConveyerBeltItem* heldItem = nullptr;
};

