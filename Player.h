#pragma once
#include "Container.h"
#include "Entity.h"

class Player : public Entity
{
public:
	Player(lost::Vector2D position);
	virtual ~Player();

	virtual void loopPosition(float worldLoopWidth);
	virtual void update();
	virtual void render(lost::Bound2D renderBounds, float worldLoopWidth);

	// Returns the amount of that item that COULDN'T be added to the container
	int addItem(Item& item);
	inline Container* getInventory() { return &inventory; };

	Item holdingItem;
	int moneyCount = 100;
	bool lockInput = false;
	bool lockMovement = false;
	bool lockActions = false;
private:
	Container inventory;
};

extern Player* g_PlayerPointer;

