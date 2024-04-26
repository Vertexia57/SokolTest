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
	~Collider();

	void update();

	inline void addVelocity(float x, float y) { m_Velocity = m_Velocity + lost::Vector2D{ x, y }; };
	inline void setVelocity(float x, float y) { m_Velocity = lost::Vector2D{ x, y }; };

	CollissionStruct sideCollissions = {};
	double timeAlive = 0.0;

	lost::Bound2D bounds;
private:
	void m_CheckCollissions();
	void m_CheckEntityCollisions(std::vector<TileEntity*>& tileEntities);

	lost::Vector2D m_Velocity;

	std::vector<Tile*> testTilesH;
	std::vector<Tile*> testTilesV;
};

