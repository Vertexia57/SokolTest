#include "FallingHub.h"
#include "World.h"
#include "Player.h"
#include "PhaseCollider.h"

FallingHub::FallingHub(lost::Vector2D position)
	: Entity({ position.x, position.y, 64.0f, 64.0f })
{
	colliderData = new PhaseCollider({ position.x, position.y, 64.0f, 64.0f });
	colliderData->gravity = false;
	colliderData->addVelocity(1000.0f, 2000.0f);
	transform.scale = { 1.0f, 1.0f };
	lost::globalCamera.bindGoalTransform(&centerTransform, 2);
	lost::globalCamera.startShake({10.0f, 10.0f});
	lost::globalCamera.setRotateFollow(false);
}

FallingHub::~FallingHub()
{
	lost::globalCamera.unbindGoalTransform(&centerTransform);
	lost::globalCamera.stopShake();
	lost::globalCamera.setRotateFollow(true);
}

void FallingHub::update()
{
	Entity::update();
	if (colliderData->sideCollissions.down)
	{
		die();
		g_World->addEntity(new Player({transform.position.x + 22.0f, transform.position.y + 22.0f - 32.0f * 4}));
		for (int x = floor(colliderData->bounds.left / 32.0f); x < ceil(colliderData->bounds.right / 32.0f); x++)
		{
			for (int y = floor(colliderData->bounds.top / 32.0f) - 1; y < ceil(colliderData->bounds.bottom / 32.0f) - 1; y++)
			{
				g_World->setTile(g_TileManager.getTileRef("air"), x, y);
			}
			g_World->setTile(g_TileManager.getTileRef("stone"), x, ceil(colliderData->bounds.bottom / 32.0f) - 1);
		}
		g_World->addTileEntity(new TileEntity(g_TileManager.getTileEntityRef("passiveGenerator")), floor(colliderData->bounds.left / 32.0f), ceil(colliderData->bounds.bottom / 32.0f) - 5);
	}
}

void FallingHub::render(lost::Bound2D renderBounds, float worldLoopWidth)
{
	if (colliderData->bounds.inBounds(renderBounds))
	{
		sgp_push_transform();
		lost::useImage(1);
		float imageWidth = lost::getImage(1)->width;
		float imageHeight = lost::getImage(1)->height;
		sgp_rotate_at(-0.4f, (float)transform.position.x + colliderData->bounds.w / 2.0f, (float)transform.position.y + colliderData->bounds.h / 2.0f);
		sgp_draw_textured_rect(0, { (float)transform.position.x, (float)transform.position.y, colliderData->bounds.w, colliderData->bounds.h }, { 0, 0, imageWidth, imageHeight });
		sgp_pop_transform();
	}
	else
	{
		float xOffset = (colliderData->bounds.x > renderBounds.right) ? -worldLoopWidth : worldLoopWidth;
		lost::Bound2D loopedBounds = { colliderData->bounds.x + xOffset, colliderData->bounds.y, colliderData->bounds.w, colliderData->bounds.h };
		if (loopedBounds.inBounds(renderBounds))
		{
			sgp_push_transform();
			lost::useImage(1);
			float imageWidth = lost::getImage(1)->width;
			float imageHeight = lost::getImage(1)->height;
			sgp_rotate_at(-0.4f, (float)transform.position.x + colliderData->bounds.w / 2.0f, (float)transform.position.y + colliderData->bounds.h / 2.0f);
			sgp_draw_textured_rect(0, { loopedBounds.x, loopedBounds.y, loopedBounds.w, loopedBounds.h }, { 0, 0, imageWidth, imageHeight });
			sgp_pop_transform();
		}
	}
}
