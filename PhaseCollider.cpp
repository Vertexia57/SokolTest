#include "PhaseCollider.h"
#include "World.h"

PhaseCollider::PhaseCollider(lost::Bound2D bound)
	: Collider(bound)
{
}

PhaseCollider::~PhaseCollider()
{
}

void PhaseCollider::update()
{
	double deltaTime = lost::deltaTime / 1000.0f;

	timeAlive += lost::deltaTime;
	bounds = bounds + velocity * deltaTime;
	if (gravity)
		velocity = velocity + lost::Vector2D{ 0.0f, 1000.0f } *deltaTime;

	m_CheckCollissions();
	bounds.calcSides();
}

void PhaseCollider::m_CheckCollissions()
{
	double deltaTime = lost::deltaTime;

	sideCollissions = {};

	std::vector<TileEntity*> testEntities = {};

	lost::Vector2D testLocation = { bounds.x + bounds.w + 1, bounds.y };
	testTilesV[0] = g_World->getTileAt(floor(testLocation.x / 32.0f), floor((testLocation.y + abs(velocity.y * deltaTime / 1000.0f) + 1) / 32.0f));
	testTilesV[1] = g_World->getTileAt(floor(testLocation.x / 32.0f), floor((testLocation.y + bounds.h - abs(velocity.y * deltaTime / 1000.0f) - 2) / 32.0f));

	for (Tile* test : testTilesV)
	{
		if (test)
		{
			if (test->referenceStruct->collidable)
			{
				sideCollissions.right = true;
				break;
			}
			else
			{
				testEntities.insert(testEntities.end(), test->tileEntitiesWithin.begin(), test->tileEntitiesWithin.end());
			}
		}
	}

	testLocation = { bounds.x - 1, bounds.y };
	testTilesV[0] = g_World->getTileAt(floor(testLocation.x / 32.0f), floor((testLocation.y + abs(velocity.y * deltaTime / 1000.0f) + 1) / 32.0f));
	testTilesV[1] = g_World->getTileAt(floor(testLocation.x / 32.0f), floor((testLocation.y + bounds.h - abs(velocity.y * deltaTime / 1000.0f) - 2) / 32.0f));
	for (Tile* test : testTilesV)
	{
		if (test)
		{
			if (test->referenceStruct->collidable)
			{
				sideCollissions.left = true;
				break;
			}
			else
			{
				testEntities.insert(testEntities.end(), test->tileEntitiesWithin.begin(), test->tileEntitiesWithin.end());
			}
		}
	}

	testLocation = { bounds.x, bounds.y + bounds.h + 1 };
	testTilesH[0] = g_World->getTileAt(floor((testLocation.x + abs(velocity.x * deltaTime / 1000.0f) + 1) / 32.0f), floor(testLocation.y / 32.0f));
	testTilesH[1] = g_World->getTileAt(floor((testLocation.x + bounds.w - abs(velocity.x * deltaTime / 1000.0f) - 2) / 32.0f), floor(testLocation.y / 32.0f));

	for (Tile* test : testTilesH)
	{
		if (test)
		{
			if (test->referenceStruct->collidable)
			{
				sideCollissions.down = true;
				break;
			}
			else
			{
				testEntities.insert(testEntities.end(), test->tileEntitiesWithin.begin(), test->tileEntitiesWithin.end());
			}
		}
	}

	testLocation = { bounds.x, bounds.y - 2 };
	testTilesH[0] = g_World->getTileAt(floor((testLocation.x + abs(velocity.x * deltaTime / 1000.0f) + 1) / 32.0f), floor(testLocation.y / 32.0f));
	testTilesH[1] = g_World->getTileAt(floor((testLocation.x + bounds.w - abs(velocity.x * deltaTime / 1000.0f) - 2) / 32.0f), floor(testLocation.y / 32.0f));
	for (Tile* test : testTilesH)
	{
		if (test)
		{
			if (test->referenceStruct->collidable)
			{
				sideCollissions.up = true;
				break;
			}
			else
			{
				testEntities.insert(testEntities.end(), test->tileEntitiesWithin.begin(), test->tileEntitiesWithin.end());
			}
		}
	}

	m_CheckEntityCollisions(testEntities);
}

void PhaseCollider::m_CheckEntityCollisions(std::vector<TileEntity*>& tileEntities)
{
	double deltaTime = lost::deltaTime;

	lost::Vector2D testLocation = { 0.0f, 0.0f };

	//std::set<TileEntity*> completedTileEntities;

	bool* completedTileEntities = new bool[tileEntities.size()];
	for (int i = 0; i < tileEntities.size(); i++)
		completedTileEntities[i] = false;

	for (int i = 0; i < tileEntities.size(); i++)
	{
		TileEntity* entity = tileEntities[i];

		if (!completedTileEntities[i] && entity->collidable)
		{
			lost::Bound2D hitbox = entity->getHitbox();
			hitbox = { hitbox.x * 32.0f, hitbox.y * 32.0f, hitbox.w * 32.0f, hitbox.h * 32.0f };

			if (hitbox.right > bounds.left + g_World->worldTileWidth * 32.0f && hitbox.left - g_World->worldTileWidth * 32.0f < bounds.left)
				hitbox = { hitbox.x - g_World->worldTileWidth * 32.0f, hitbox.y, hitbox.w, hitbox.h };

			if (bounds.inBounds(hitbox))
			{

				std::array<float, 4> pushDist = {
					bounds.x + bounds.w - 1 - hitbox.x,
					bounds.x - (hitbox.x + hitbox.w - 1),
					bounds.y + bounds.h - 1 - hitbox.y,
					bounds.y - (hitbox.y + hitbox.h - 1)
				};

				uint32_t lowestIndex = 0;
				float lowestValue = pushDist[0];
				for (uint32_t i = 1; i < pushDist.size(); i++)
				{
					if (fabsf(pushDist[i]) < lowestValue)
					{
						lowestValue = fabsf(pushDist[i]);
						lowestIndex = i;
					}
				}

				switch (lowestIndex)
				{
				case 0:
					sideCollissions.right = true;
					break;
				case 1:
					sideCollissions.left = true;
					break;
				case 2:
					sideCollissions.down = true;
					break;
				case 3:
					sideCollissions.up = true;
					break;
				}

			}

			completedTileEntities[i] = true;
		}
	}

	delete[] completedTileEntities;
}
