#pragma once
#include "JSON.h"
#include "Lost.h"
#include "Collider.h"

class Entity
{
public:
	Entity(lost::Bound2D bounds);
	virtual ~Entity();
	
	virtual void loopPosition(float worldLoopWidth);

	virtual void update();
	virtual void render(lost::Bound2D renderBounds, float worldLoopWidth);
	virtual void die(); // Run deletion with extra stuff like drops and particles

	bool killEntity = false;
protected:
	lost::Transform2D transform;

	Collider* colliderData;
	JSONObject* jsonData = nullptr;

	// [!] TODO: Add a "Contains Item" array here, which each entity can carry. This will be used by factories and processes
	// [!]       std::vector<ItemRef*> containsItem;
};

