#include "Entity.h"
#include "random.h"

Entity::Entity(lost::Bound2D bounds)
{
	transform.position = { bounds.x, bounds.y };
}

Entity::~Entity()
{
	delete colliderData;
}

void Entity::loopPosition(float worldLoopWidth)
{
	if (colliderData->bounds.left > worldLoopWidth)
		colliderData->addPosition(-worldLoopWidth, 0.0f);
	else if (colliderData->bounds.right < 0)
		colliderData->addPosition(worldLoopWidth, 0.0f);
}

void Entity::update()
{
	colliderData->update();
	transform.position = { colliderData->bounds.x, colliderData->bounds.y };
	centerTransform = transform;
	centerTransform.position = centerTransform.position + lost::Vector2D{ colliderData->bounds.w / 2.0f, colliderData->bounds.h / 2.0f };
	if (transform.position.y >= 200.0f * 32.0f)
		killEntity = true;
}

void Entity::render(lost::Bound2D renderBounds, float worldLoopWidth)
{
	if (colliderData->bounds.inBounds(renderBounds))
	{
		lost::useImage(1);
		float imageWidth = lost::getImage(1)->width;
		float imageHeight = lost::getImage(1)->height;
		sgp_draw_textured_rect(0, { (float)transform.position.x, (float)transform.position.y, colliderData->bounds.w, colliderData->bounds.h }, { 0, 0, imageWidth, imageHeight });
	}
	else
	{
		float xOffset = (colliderData->bounds.x > renderBounds.right) ? -worldLoopWidth : worldLoopWidth;
		lost::Bound2D loopedBounds = { colliderData->bounds.x + xOffset, colliderData->bounds.y, colliderData->bounds.w, colliderData->bounds.h };
		if (loopedBounds.inBounds(renderBounds))
		{
			lost::useImage(1);
			float imageWidth = lost::getImage(1)->width;
			float imageHeight = lost::getImage(1)->height;
			sgp_draw_textured_rect(0, { loopedBounds.x, loopedBounds.y, loopedBounds.w, loopedBounds.h }, { 0, 0, imageWidth, imageHeight });
		}
	}
}

void Entity::die()
{
	killEntity = true;
}

lost::Vector2D Entity::getPosition()
{
	return lost::Vector2D{ colliderData->bounds.x + colliderData->bounds.w / 2, colliderData->bounds.y + colliderData->bounds.h / 2 };
}
