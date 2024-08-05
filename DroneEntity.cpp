#include "DroneEntity.h"
#include "DroneBay.h"

DroneEntity::DroneEntity(lost::Vector2D position, DroneBay* goalBay)
	: Entity({ position.x, position.y, 30.0f, 30.0f })
{
	colliderData = new Collider({ position.x, position.y, 20.0f, 20.0f });
	colliderData->gravity = false;
	m_GoalBay = goalBay;
	m_Container = new Container(1);
	std::vector<lost::Vector2D> path = { { position.x, position.y }, { position.x, 100.0f }, { goalBay->position.x * 32.0f + goalBay->getHitbox().w * 32.0f / 2.0f, 100.0f}, {goalBay->position.x * 32.0f + goalBay->getHitbox().w * 32.0f / 2.0f, goalBay->position.y * 32.0f}};
	m_Path = NodePath(path);

	m_GoalBay->addIncomingDrone(this);
}

DroneEntity::~DroneEntity()
{
	delete m_Container;
	m_GoalBay->removeIncomingDrone(this);
}

void DroneEntity::update()
{

	//m_Path.currentGoal = lost::globalCamera.screenToWorld(lost::mousePos());
	
	if (m_Path.current >= m_Path.goals.size() - 3)
	{
		// Calculate accelleration required to reach the goal node

		//                         2 * (goal - position - startVel)      < this is "diff"
		// A(startPos, startVel) = -------------------------------- * dt < dt is in seconds
		//                                 timeToReach ^ 0.5             < timeToReach is distanceToGoal / speed

		// This makes the drone accellerate in the opposite direction it's moving in if, 
		// in the time it would take to reach that node it would have a |velocity| > 0

		float timeToReachNode = fmaxf(m_Path.currentGoal.dist(getPosition()) / m_Speed, 0.001f);

		lost::Vector2D diff = (m_Path.currentGoal - getPosition() - colliderData->velocity) * 2.0f;
		lost::Vector2D newAccelleration = ((diff / sqrtf(timeToReachNode))) * lost::deltaTime / 1000.0;

		colliderData->addVelocity(newAccelleration.x, newAccelleration.y);
	}
	else
	{
		lost::Vector2D moveVec = (m_Path.currentGoal - getPosition()).normalized() * m_Speed * lost::deltaTime / 1000.0;
		colliderData->addVelocity(moveVec.x, moveVec.y);
	}

	if (colliderData->velocity.magnitude() > m_Speed)
	{
		colliderData->velocity.normalize();
		colliderData->velocity = colliderData->velocity * m_Speed;
	}

	m_DistanceTraveled += colliderData->velocity.magnitude() * lost::deltaTime / 1000.0;
	if (m_Path.currentGoal.dist(getPosition()) < (m_Path.current == 3 ? 50.0f : 300.0f))
	{
		m_DistanceTraveled = 0.0f;
		if (m_Path.current == 3)
		{
			if (m_GoalBay->getInventory()->getItem(0)->empty)
				m_GoalBay->insertItem(*m_Container->getItem(0));
			else
				m_GoalBay->getInventory()->getItem(0)->StackSize += m_Container->getItem(0)->StackSize;

			die();
		}
		m_Path.next();
	}

	Entity::update();
}

void DroneEntity::render(lost::Bound2D renderBounds, float worldLoopWidth)
{
	Entity::render(renderBounds, worldLoopWidth);
}

int DroneEntity::insertItem(Item& item)
{
	return m_Container->addItem(item);
}

bool DroneEntity::canInsert(Item& item)
{
	return m_Container->findAccessableSlot(item) != -1;
}
