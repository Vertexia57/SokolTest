#include "FallingHub.h"
#include "World.h"
#include "Player.h"
#include "PhaseCollider.h"
#include "HubTileEntity.h"

FallingHub::FallingHub(lost::Vector2D position)
	: Entity({ position.x, position.y, 96.0f, 128.0f })
{
	colliderData = new PhaseCollider({ position.x, position.y, 96.0f, 128.0f });
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
		int floorY = ceil(colliderData->bounds.bottom / 32.0f);

		bool inAir = true;
		for (int x = floor(colliderData->bounds.left / 32.0f); x < ceil(colliderData->bounds.right / 32.0f) - 1; x++)
		{
			// Check all tiles underneath area to build and check if they're empty
			if (!g_World->getTileAt(x, floorY - 1)->empty)
				inAir = false;
		}

		// If they are empty, move the building down one tile
		if (inAir)
			floorY++;

		g_World->addEntity(new Player({transform.position.x + 22.0f, transform.position.y + 22.0f - 32.0f * 4}));
		for (int x = floor(colliderData->bounds.left / 32.0f); x < ceil(colliderData->bounds.right / 32.0f) - 1; x++)
		{
			for (int y = floor(colliderData->bounds.top / 32.0f); y < floorY - 1; y++)
			{
				g_World->setTile(g_TileManager.getTileRef("air"), x, y);
			}
			g_World->setTile(g_TileManager.getTileRef("stone"), x, floorY - 1);
		}
		HubTileEntity* entity = new HubTileEntity(g_TileManager.getTileEntityRef("passiveGenerator"));
		g_World->addTileEntity(entity, floor(colliderData->bounds.left / 32.0f), floorY - 5);
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
