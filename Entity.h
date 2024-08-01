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

	lost::Vector2D getPosition();

	bool killEntity = false;
protected:
	lost::Transform2D transform;
	lost::Transform2D centerTransform;

	Collider* colliderData = nullptr;
	JSONObject* jsonData = nullptr;

	// [!] TODO: Add a "Contains Item" array here, which each entity can carry. This will be used by factories and processes
	// [!]       std::vector<ItemRef*> containsItem;
};

