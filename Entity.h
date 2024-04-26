#pragma once
#include "JSON.h"
#include "Lost.h"
#include "Collider.h"

class Entity
{
public:
	Entity(lost::Bound2D bounds);
	~Entity();
	
	void update();
	void render(lost::Bound2D renderBounds);
	void die(); // Run deletion with extra stuff like drops and particles

	bool killEntity = false;
private:
	lost::Transform2D transform;

	Collider* colliderData;
	JSONObject* jsonData = nullptr;
};

