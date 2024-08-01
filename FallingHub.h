#pragma once
#include "Entity.h"

class FallingHub : public Entity
{
public:
	FallingHub(lost::Vector2D position);
	~FallingHub();

	virtual void update();
	virtual void render(lost::Bound2D renderBounds, float worldLoopWidth);
protected:
};

