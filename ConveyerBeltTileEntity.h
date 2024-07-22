#pragma once
#include "TileEntity.h"
#include "Item.h"

// [!] TODO: Figure this out, maybe use a pen and paper
struct ConveyerBeltItem
{
	Item item;
	float x;
	float y;
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

	void checkNeighbors();

	virtual void insertItem(Item& item);
	virtual bool canInsert(Item& item) const;

	void checkPasses();
	void passItem(ConveyerBeltTileEntity* other, lost::IntVector2D tileOffset);

	inline bool getEmpty() const { return (heldItem == nullptr || m_Moving); };
protected:
	ConveyerBeltTileEntity* m_Left = nullptr;
	ConveyerBeltTileEntity* m_Right = nullptr;
	ConveyerBeltTileEntity* m_Up = nullptr;
	ConveyerBeltTileEntity* m_Down = nullptr;

	lost::Vector2D m_Speeds = { 0.0f, 0.0f };
	bool m_Diagonal = false;

	bool m_Moving = false;
	ConveyerBeltItem* heldItem = nullptr;
};

