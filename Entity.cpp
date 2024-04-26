#include "Entity.h"
#include "random.h"

Entity::Entity(lost::Bound2D bounds)
{
	colliderData = new Collider(bounds);

	transform.position = { colliderData->bounds.x, colliderData->bounds.y };
}

Entity::~Entity()
{
	delete colliderData;
}

void Entity::update()
{
	colliderData->update();
	transform.position = { colliderData->bounds.x, colliderData->bounds.y };
	if (transform.position.y >= 200.0f * 32.0f)
		killEntity = true;
}

void Entity::render(lost::Bound2D renderBounds)
{
	lost::useImage(1);
	float imageWidth = lost::getImage(1)->width;
	float imageHeight = lost::getImage(1)->height;
	sgp_draw_textured_rect(0, { (float)transform.position.x, (float)transform.position.y, colliderData->bounds.w, colliderData->bounds.h }, { 0, 0, imageWidth, imageHeight });
}

void Entity::die()
{
}
