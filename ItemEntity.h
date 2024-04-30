#pragma once
#include "Entity.h"
#include "Item.h"

class ItemEntity : public Entity
{
public:
	ItemEntity(lost::Vector2D position, Item& item);
	ItemEntity(lost::Vector2D position, ItemRefStruct* item, int count);
	virtual ~ItemEntity();

	virtual void loopPosition(float worldLoopWidth);
	virtual void update();
	virtual void render(lost::Bound2D renderBounds, float worldLoopWidth);

	Item itemCarried;
protected:
	bool m_Following = false;
	int m_WorldLoopWidth = 0;
};

