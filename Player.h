#pragma once
#include "Entity.h"

class Player : public Entity
{
public:
	Player(lost::Vector2D position);
	virtual ~Player();

	virtual void loopPosition(float worldLoopWidth);
	virtual void update();
	virtual void render(lost::Bound2D renderBounds);
private:
};

