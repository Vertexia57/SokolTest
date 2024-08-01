#pragma once
#include <vector>
#include <set>
#include "Lost.h"
#include "Tile.h"

struct CollissionStruct
{
	bool up = false;
	bool right = false;
	bool down = false;
	bool left = false;
};

class Collider
{
public:
	Collider(lost::Bound2D bound);
	virtual ~Collider();

	virtual void update();

	inline void addVelocity(float x, float y) { velocity = velocity + lost::Vector2D{ x, y }; };
	inline void setVelocity(float x, float y) { velocity = lost::Vector2D{ x, y }; };

	inline void addPosition(float x, float y) { bounds.x += x; bounds.y += y; bounds.calcSides(); };

	CollissionStruct sideCollissions = {};
	double timeAlive = 0.0;

	lost::Bound2D bounds;
	bool gravity = true;

	lost::Vector2D velocity;
protected:
	virtual void m_CheckCollissions();
	virtual void m_CheckEntityCollisions(std::vector<TileEntity*>& tileEntities);

	std::vector<Tile*> testTilesH;
	std::vector<Tile*> testTilesV;
};

