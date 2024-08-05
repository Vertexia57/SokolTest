#pragma once
#include "Entity.h"

class DroneBay;

class DroneEntity : public Entity
{
protected:
	struct NodePath
	{
		std::vector<lost::Vector2D> goals;
		std::vector<float> distances = {};

		NodePath() {};

		NodePath(std::vector<lost::Vector2D>& goals_, lost::Vector2D currentPos)
		{
			goals = goals_;
			distances.push_back(currentPos.dist(goals[0]));
			for (int i = 0; i < goals.size() - 1; i++)
				distances.push_back(goals[i].dist(goals[i + 1]));

			current = 1;
			currentGoal = goals[current];
			thisDistance = distances[current];
		}

		NodePath(std::vector<lost::Vector2D>& goals_)
		{
			goals = goals_;
			current = 1;
			for (int i = 0; i < goals.size() - 1; i++)
				distances.push_back(goals[i].dist(goals[i + 1]));

			currentGoal = goals[current];
			thisDistance = distances[current - 1];
		}

		size_t current = 0;
		lost::Vector2D currentGoal = { 0.0f, 0.0f };
		float thisDistance = 0.0f;
		void next()
		{
			if (current < goals.size() - 1)
			{
				current++;
				currentGoal = goals[current];
				thisDistance = distances[current - 1];
			}
		}
	};

public:
	DroneEntity(lost::Vector2D position, DroneBay* goalBay);
	~DroneEntity();

	virtual void update();
	virtual void render(lost::Bound2D renderBounds, float worldLoopWidth);

	// Returns how many items couldn't be inserted
	int insertItem(Item& item);
	// Returns if that item can be inserted
	bool canInsert(Item& item);
protected:
	DroneBay* m_GoalBay = nullptr;
	Container* m_Container = nullptr;

	NodePath m_Path;
	float m_DistanceTraveled = 0.0f;
	float m_Speed = 500.0f;
};

