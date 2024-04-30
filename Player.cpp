#include "Player.h"

Player::Player(lost::Vector2D position)
	: Entity({ position.x, position.y, 20.0f, 20.0f })
	, inventory(10)
{
	colliderData->gravity = false;
	transform.scale = { 0.5f, 0.5f }; // The scale is only used by the camera
	lost::globalCamera.bindGoalTransform(&transform, 3);
}

Player::~Player()
{
	lost::globalCamera.unbindGoalTransform(&transform);
}

void Player::loopPosition(float worldLoopWidth)
{
	if (colliderData->bounds.left > worldLoopWidth)
	{
		colliderData->addPosition(-worldLoopWidth, 0.0f);
		if (lost::globalCamera.hasFocus(&transform))
			lost::globalCamera.addPosition(-worldLoopWidth, 0.0f);
	}
	else if (colliderData->bounds.right < 0)
	{
		colliderData->addPosition(worldLoopWidth, 0.0f);
		if (lost::globalCamera.hasFocus(&transform))
			lost::globalCamera.addPosition(worldLoopWidth, 0.0f);
	}
}

void Player::update()
{
	Entity::update();

	if (!ImGui::IsAnyItemActive())
	{
		if (lost::keyDown(SAPP_KEYCODE_A))
			colliderData->addVelocity(-5.0f, 0.0f);
		if (lost::keyDown(SAPP_KEYCODE_D))
			colliderData->addVelocity(5.0f, 0.0f);
		if (lost::keyDown(SAPP_KEYCODE_W))
			colliderData->addVelocity(0.0f, -5.0f);
		if (lost::keyDown(SAPP_KEYCODE_S))
			colliderData->addVelocity(0.0f, 5.0f);
	}

	colliderData->velocity = colliderData->velocity * (1.0 - (2.0 * lost::deltaTime / 1000.0f));

	ImGui::Text("Player Position: (%f, %f)", colliderData->bounds.x, colliderData->bounds.y);
}

void Player::render(lost::Bound2D renderBounds, float worldLoopWidth)
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

int Player::addItem(Item& item)
{
	return inventory.addItem(item);
}

Player* g_PlayerPointer = nullptr;