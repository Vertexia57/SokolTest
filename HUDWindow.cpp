#include "HUDWindow.h"
#include "Player.h"
#include "Lost/FontManager.h"
#include "World.h"
#include "Lost/UIManager.h"
#include "ItemEntity.h"
#include "ItemManager.h"

HUDWindow::HUDWindow(lost::Bound2D bounds)
	: UIWindow(bounds)
{
	m_TopLeftUITexID = lost::getImageID("HUDTopLeft");
	orderValue = 5;
	windowType = "HUD";
	maxOne = true;

	m_BuildMenuAnim = m_Animator.addAnimation(lost::EaseMode::OUT, 0.5f);
	m_BuildMenuAnim->setVal(76.0f * 2.0f);

	for (int i = 0; i < 4; i++)
		m_BuildSubMenuAnims[i] = m_Animator.addAnimation(lost::EaseMode::OUT, 0.5f);

	m_SubMenuIconAnim = m_Animator.addAnimationChain({
		lost::Animation(lost::EaseMode::OUT, 0.2), 
		lost::Animation(lost::EaseMode::OUT, 0.3), 
	});

	(*m_SubMenuIconAnim)[0].setStart(1.0f);
	(*m_SubMenuIconAnim)[0].setEnd(0.0f);
	(*m_SubMenuIconAnim)[1].setStart(0.0f);
	(*m_SubMenuIconAnim)[1].setEnd(1.0f);
}

HUDWindow::~HUDWindow()
{
}

void HUDWindow::update()
{
	m_Animator.tick();

	m_Bounds = { 0.0f, 0.0f, (float)sapp_width(), (float)sapp_height() };
	selected = false;

	if (lost::keyTapped(SAPP_KEYCODE_B))
	{
		m_Building = !m_Building;
		if (m_Building)
			m_Rotation = 0;
		if (!g_PlayerPointer->lockActions && m_Building)
		{
			g_PlayerPointer->lockActions = true;
			m_BuildMenuAnim->setGoal(0.0f);
		}
		if (g_PlayerPointer->lockActions && !m_Building)
		{
			g_PlayerPointer->lockActions = false;
			m_BuildMenuAnim->setGoal(85.0f * 2.0f);
		}
	}

	if (lost::keyTapped(SAPP_KEYCODE_E))
		g_PlayerPointer->openInventory({ 0.0f, 0.0f }, true);

	if (!m_Building)
	{
		lost::Vector2D worldMouse = lost::globalCamera.screenToWorld(lost::mousePos());
		lost::Vector2D blockMouse = { floor(worldMouse.x / 32.0f), floor(worldMouse.y / 32.0f) };
		Tile* tileHovered = g_World->getTileAt(floor(worldMouse.x / 32.0f), floor(worldMouse.y / 32.0f));

		for (int i = tileHovered->tileEntitiesWithin.size() - 1; i >= 0; i--)
		{
			lost::unbindShader();
			lost::clearImage();

			sgp_set_color(1.0f, 1.0f, 1.0f, 1.0f);
			tileHovered->tileEntitiesWithin[i]->renderHitbox();
			if (!lost::isUISelected())
			{
				tileHovered->tileEntitiesWithin[i]->mouseInteractFunction();
			}

			lost::bindShader(lost::getShader(0));
		}
	}
	else
	{
		int buildingGroupCount = g_TileManager.buildingGroups.size();
		if (m_SelectedGroupSlot != -1)
		{
			int buildingCount = g_TileManager.buildingGroups[m_SelectedGroupSlot].buildingRefs.size();

			m_HoveredBuildingSlot = (lost::mousePos().y >= m_Bounds.h - 1.0f - 76.0f * 2 && lost::mousePos().y <= m_Bounds.h - 1.0f - 76.0f) ? floor((lost::mousePos().x - (m_Bounds.w - buildingCount * 76.0f) / 2.0f + (buildingGroupCount - 1) * 76.0f / 2.0f - m_SelectedGroupSlot * 76.0f) / 76.0f) : -1;
			if (m_HoveredBuildingSlot >= 0 && m_HoveredBuildingSlot < buildingCount)
			{
				selected = true;
				if (lost::mouseTapped(0))
				{
					m_SelectedBuilding = g_TileManager.buildingGroups[m_SelectedGroupSlot].buildingRefs[m_HoveredBuildingSlot];
					m_SelectedBuildingSlot = m_HoveredBuildingSlot;
					m_Rotation = 0;
				}
			}
			else
			{
				m_HoveredBuildingSlot = -1;
			}
		}

		m_HoveredGroupSlot = (lost::mousePos().y >= m_Bounds.h - 1.0f - 76.0f) ? floor((lost::mousePos().x - (m_Bounds.w - buildingGroupCount * 76.0f) / 2.0f) / 76.0f) : -1;
		if (m_HoveredGroupSlot >= 0 && m_HoveredGroupSlot < buildingGroupCount)
		{
			selected = true;
			if (lost::mouseTapped(0) && m_HoveredGroupSlot != m_SelectedGroupSlot)
			{
				m_OldGroupSlot = m_SelectedGroupSlot;
				m_SelectedGroupSlot = m_HoveredGroupSlot;
				if (!m_BuildSubMenuOpen)
				{
					// Set the values of the bounds animations
					m_BuildSubMenuAnims[0]->setVal(
						(m_Bounds.w - buildingGroupCount * 76.0f) / 2.0f + m_SelectedGroupSlot * 76.0f // buildingSlotX
						- ((g_TileManager.buildingGroups[m_SelectedGroupSlot].buildingRefs.size() - 1) * 76.0f / 2)); // offset
					m_BuildSubMenuAnims[1]->setVal(m_Bounds.h - 2 * 76.0f);
					m_BuildSubMenuAnims[2]->setVal(g_TileManager.buildingGroups[m_SelectedGroupSlot].buildingRefs.size() * 76.0f);
					m_BuildSubMenuAnims[3]->setVal(76.0f);
				}
				else
				{
					// Set the end values of the bounds animations
					m_BuildSubMenuAnims[0]->setGoal(
						(m_Bounds.w - buildingGroupCount * 76.0f) / 2.0f + m_SelectedGroupSlot * 76.0f // buildingSlotX
						- ((g_TileManager.buildingGroups[m_SelectedGroupSlot].buildingRefs.size() - 1) * 76.0f / 2)); // offset
					m_BuildSubMenuAnims[1]->setGoal(m_Bounds.h - 2 * 76.0f);
					m_BuildSubMenuAnims[2]->setGoal(g_TileManager.buildingGroups[m_SelectedGroupSlot].buildingRefs.size() * 76.0f);
					m_BuildSubMenuAnims[3]->setGoal(76.0f);
				}
				m_BuildSubMenuOpen = true;
				m_SubMenuIconAnim->restart();
				m_SelectedBuildingSlot = -1;
			}
		}

		if (lost::keyTapped(SAPP_KEYCODE_R) && m_SelectedBuilding->totalRotations != 0)
			m_Rotation = (m_Rotation + 1) % m_SelectedBuilding->totalRotations;

		lost::Vector2D worldMouse = lost::globalCamera.screenToWorld(lost::mousePos());
		lost::Vector2D blockMouse = { floor(worldMouse.x / 32.0f), floor(worldMouse.y / 32.0f) };

		Tile* tileHovered = g_World->getTileAt(floor(worldMouse.x / 32.0f), floor(worldMouse.y / 32.0f));
		if (tileHovered)
		{

			for (int i = tileHovered->tileEntitiesWithin.size() - 1; i >= 0; i--)
			{
				lost::unbindShader();
				lost::clearImage();

				sgp_set_color(1.0f, 1.0f, 1.0f, 1.0f);
				tileHovered->tileEntitiesWithin[i]->renderHitbox();
				if (!lost::isUISelected() && lost::mouseDown(1))
				{
					g_PlayerPointer->moneyCount += tileHovered->tileEntitiesWithin[i]->tileEntityRef->cost;
					g_World->destroyTileEntity(tileHovered->tileEntitiesWithin[i]);
				}

				lost::bindShader(lost::getShader(0));
			}

			if (!lost::isUISelected() && m_SelectedBuilding)
			{
				lost::useImage(m_SelectedBuilding->texture);
				float imageWidth = lost::getImage(m_SelectedBuilding->texture)->width / m_SelectedBuilding->totalFrames;
				float imageHeight = lost::getImage(m_SelectedBuilding->texture)->height / m_SelectedBuilding->totalVariants;

				if (m_CanPlaceBuilding)
					sgp_set_color(1.0f, 1.0f, 1.0f, 0.2f);
				else
					sgp_set_color(1.0f, 0.1f, 0.1f, 0.2f);

				if (m_SelectedBuilding->rotationVariants.size() > 0)
					sgp_draw_textured_rect(0, { (blockMouse.x + m_SelectedBuilding->placementOffsetX) * 32.0f, (blockMouse.y + m_SelectedBuilding->placementOffsetY) * 32.0f, m_SelectedBuilding->width * 32.0f, m_SelectedBuilding->height * 32.0f }, { 0, imageHeight * m_SelectedBuilding->rotationVariants[m_Rotation], imageWidth, imageHeight });
				else
					sgp_draw_textured_rect(0, { (blockMouse.x + m_SelectedBuilding->placementOffsetX) * 32.0f, (blockMouse.y + m_SelectedBuilding->placementOffsetY) * 32.0f, m_SelectedBuilding->width * 32.0f, m_SelectedBuilding->height * 32.0f }, { 0, imageHeight * m_Rotation, imageWidth, imageHeight });
				lost::clearImage();

				m_CanPlaceBuilding = (!m_SelectedBuilding->requiresSupport || g_World->checkStable(
					{ blockMouse.x + m_SelectedBuilding->placementOffsetX, blockMouse.y + 1, m_SelectedBuilding->width, 1.0f}
				)) && (g_World->checkCanPlace(
					{blockMouse.x + m_SelectedBuilding->placementOffsetX, blockMouse.y + m_SelectedBuilding->placementOffsetY, m_SelectedBuilding->width, m_SelectedBuilding->height }, m_SelectedBuilding->fillsLayers
				));
				if (lost::mouseDown(0) && m_CanPlaceBuilding && g_PlayerPointer->moneyCount >= m_SelectedBuilding->cost)
				{
					createBuilding(m_SelectedBuilding, blockMouse, m_Rotation);
					g_PlayerPointer->moneyCount -= m_SelectedBuilding->cost;
				}
			}
			
			if (!lost::isUISelected() && lost::mouseTapped(2))
			{
				g_World->addEntity(new ItemEntity({ worldMouse.x, worldMouse.y }, g_ItemManager.getItemData("ironOre"), 1));
			}

		}

	}

	sgp_set_color(1.0f, 1.0f, 1.0f, 1.0f);
}

void HUDWindow::render()
{
	if (g_PlayerPointer)
	{
		lost::useImage(m_TopLeftUITexID);
		float imageWidth = lost::getImage(m_TopLeftUITexID)->width;
		float imageHeight = lost::getImage(m_TopLeftUITexID)->height;
		sgp_draw_textured_rect(0, { 0.0f, 0.0f, imageWidth * 4.0f, imageHeight * 4.0f }, { 0.0f, 0.0f, imageWidth, imageHeight });
		// Money Count
		lost::renderTextPro(std::to_string(g_PlayerPointer->moneyCount), { 12.0f * 4.0f, 29.0f * 4.0f }, 1.0f, LOST_TEXT_ALIGN_LEFT, LOST_TEXT_ALIGN_MIDDLE);

		if (m_Building || !m_BuildMenuAnim->getComplete())
		{
			int buildingGroupCount = g_TileManager.buildingGroups.size();
			for (int i = 0; i < buildingGroupCount; i++)
			{
				BuildingGroup& group = g_TileManager.buildingGroups[i];
				lost::clearImage();
				if (m_SelectedGroupSlot == i)
					sgp_set_color(0.7f, 0.7f, 0.7f, 0.2f);
				else if (m_HoveredGroupSlot != i)
					sgp_set_color(0.0f, 0.0f, 0.0f, 0.2f);
				else
					sgp_set_color(0.5f, 0.5f, 0.5f, 0.2f);

				sgp_draw_filled_rect((m_Bounds.w - buildingGroupCount * 76.0f) / 2.0f + i * 76.0f, m_Bounds.h - 76.0f + m_BuildMenuAnim->getVal(), 76.0f, 76.0f);

				sgp_set_color(1.0f, 1.0f, 1.0f, 1.0f);
				lost::useImage(group.groupIcon);
				float iconWidth = lost::getImage(group.groupIcon)->width;
				float iconHeight = lost::getImage(group.groupIcon)->height;
				sgp_draw_textured_rect(0,
					{ (m_Bounds.w - buildingGroupCount * 76.0f) / 2.0f + 6.0f + i * 76.0f, m_Bounds.h - 70.0f + m_BuildMenuAnim->getVal(), 64.0f, 64.0f },
					{ 0.0f, 0.0f, iconWidth, iconHeight }
				);
			}

			if (m_BuildSubMenuOpen || !m_BuildMenuAnim->getComplete() && m_SelectedGroupSlot != -1)
			{
				lost::clearImage();
				sgp_set_color(0.0f, 0.0f, 0.0f, 0.4f);
				sgp_draw_filled_rect(m_BuildSubMenuAnims[0]->getVal(), m_BuildSubMenuAnims[1]->getVal() + m_BuildMenuAnim->getVal(), m_BuildSubMenuAnims[2]->getVal(), m_BuildSubMenuAnims[3]->getVal());

				if ((*m_SubMenuIconAnim)[0].getComplete())
				{
					int buildingCount = g_TileManager.buildingGroups[m_SelectedGroupSlot].buildingRefs.size();
					if (m_SelectedBuildingSlot != -1)
					{
						sgp_set_color(0.5f, 0.5f, 0.5f, (*m_SubMenuIconAnim)[1].getVal() * 0.4);
						sgp_draw_filled_rect((m_Bounds.w - buildingCount * 76.0f) / 2.0f + m_SelectedBuildingSlot * 76.0f - (buildingGroupCount - 1) * 76.0f / 2.0f + m_SelectedGroupSlot * 76.0f, m_Bounds.h - 76.0f * 2.0f + m_BuildMenuAnim->getVal(), 76.0f, 76.0f);
					}
					if (m_HoveredBuildingSlot != -1 && m_HoveredBuildingSlot != m_SelectedBuildingSlot)
					{
						sgp_set_color(0.3f, 0.3f, 0.3f, (*m_SubMenuIconAnim)[1].getVal() * 0.4);
						sgp_draw_filled_rect((m_Bounds.w - buildingCount * 76.0f) / 2.0f + m_HoveredBuildingSlot * 76.0f - (buildingGroupCount - 1) * 76.0f / 2.0f + m_SelectedGroupSlot * 76.0f, m_Bounds.h - 76.0f * 2.0f + m_BuildMenuAnim->getVal(), 76.0f, 76.0f);
					}

					for (int i = 0; i < buildingCount; i++)
					{
						TileEntityStruct* buildingRef = g_TileManager.buildingGroups[m_SelectedGroupSlot].buildingRefs[i];

						int maxImageLength = std::max(lost::getImage(buildingRef->texture)->width / buildingRef->totalFrames, lost::getImage(buildingRef->texture)->height / buildingRef->totalVariants);
						int maxTileLength = std::max(buildingRef->width, buildingRef->height);
						float scale = 1.0f / ((float)maxTileLength);
						float imageWidth = (float)lost::getImage(buildingRef->texture)->width / buildingRef->totalFrames;
						float imageHeight = (float)lost::getImage(buildingRef->texture)->height / buildingRef->totalVariants;

						sgp_set_color(1.0f, 1.0f, 1.0f, (*m_SubMenuIconAnim)[1].getVal());
						lost::useImage(buildingRef->texture);
						sgp_draw_textured_rect(0,
							{ (m_Bounds.w - buildingCount * 76.0f) / 2.0f + 6.0f + i * 76.0f + (32.0f - buildingRef->width * 64.0f * scale / 2.0f) - (buildingGroupCount - 1) * 76.0f / 2.0f + m_SelectedGroupSlot * 76.0f, m_Bounds.h + (32.0f - buildingRef->height * 64.0f * scale / 2.0f) - 70.0f - 76.0f + m_BuildMenuAnim->getVal(), buildingRef->width * 64.0f * scale, buildingRef->height * 64.0f * scale },
							{ 0.0f, 0.0f, imageWidth, imageHeight }
						);

						if (i == m_HoveredBuildingSlot)
						{
							lost::clearImage();
							lost::renderTextPro(buildingRef->name,
								{ (m_Bounds.w - buildingCount * 76.0f + 76.0f) / 2.0f + i * 76.0f - (buildingGroupCount - 1) * 76.0f / 2.0f + m_SelectedGroupSlot * 76.0f, m_Bounds.h - 74.0f - 76.0f + m_BuildMenuAnim->getVal() }, 0.5f,
								LOST_TEXT_ALIGN_MIDDLE, LOST_TEXT_ALIGN_BOTTOM);
						}
					}
				}
				else if (!(*m_SubMenuIconAnim)[0].getComplete() && m_OldGroupSlot != -1)
				{
					int buildingCount = g_TileManager.buildingGroups[m_OldGroupSlot].buildingRefs.size();
					for (int i = 0; i < buildingCount; i++)
					{
						TileEntityStruct* buildingRef = g_TileManager.buildingGroups[m_OldGroupSlot].buildingRefs[i];

						int maxImageLength = std::max(lost::getImage(buildingRef->texture)->width / buildingRef->totalFrames, lost::getImage(buildingRef->texture)->height / buildingRef->totalVariants);
						int maxTileLength = std::max(buildingRef->width, buildingRef->height);
						float scale = 1.0f / ((float)maxTileLength);
						float imageWidth = (float)lost::getImage(buildingRef->texture)->width / buildingRef->totalFrames;
						float imageHeight = (float)lost::getImage(buildingRef->texture)->height / buildingRef->totalVariants;

						sgp_set_color(1.0f, 1.0f, 1.0f, (*m_SubMenuIconAnim)[1].getVal());
						lost::useImage(buildingRef->texture);
						sgp_draw_textured_rect(0,
							{ (m_Bounds.w - buildingCount * 76.0f) / 2.0f + 6.0f + i * 76.0f + (32.0f - buildingRef->width * 64.0f * scale / 2.0f) - (buildingGroupCount - 1) * 76.0f / 2.0f + m_OldGroupSlot * 76.0f, m_Bounds.h - 70.0f - 76.0f + m_BuildMenuAnim->getVal(), buildingRef->width * 64.0f * scale, buildingRef->height * 64.0f * scale },
							{ 0.0f, 0.0f, imageWidth, imageHeight }
						);

						//lost::clearImage();
						//lost::renderTextPro(std::to_string((int)buildingRef->cost),
						//	{ (m_Bounds.w - buildingCount * 76.0f + 76.0f) / 2.0f + i * 76.0f - (buildingGroupCount - 1) * 76.0f / 2.0f + m_OldGroupSlot * 76.0f, m_Bounds.h - 74.0f - 76.0f + m_BuildMenuAnim->getVal() }, 1.0f,
						//	LOST_TEXT_ALIGN_MIDDLE, LOST_TEXT_ALIGN_BOTTOM);
					}
				}

			}
		}

		if (!g_PlayerPointer->holdingItem.empty)
		{

			lost::useImage(g_PlayerPointer->holdingItem.textureID);
			float imageWidth = lost::getImage(g_PlayerPointer->holdingItem.textureID)->width / g_PlayerPointer->holdingItem.refStruct->frames;
			float imageHeight = lost::getImage(g_PlayerPointer->holdingItem.textureID)->height / g_PlayerPointer->holdingItem.refStruct->variants;

			sgp_rect renderArea = { lost::mousePos().x - 32.0f, lost::mousePos().y - 32.0f, 64.0f, 64.0f };

			sgp_draw_textured_rect(0, renderArea, { 0, imageHeight * g_PlayerPointer->holdingItem.variant, imageWidth, imageHeight });
			lost::renderTextPro(
				std::to_string(g_PlayerPointer->holdingItem.StackSize),
				{ lost::mousePos().x + 32.0f, lost::mousePos().y + 32.0f },
				0.5,
				LOST_TEXT_ALIGN_RIGHT, LOST_TEXT_ALIGN_MIDDLE
			);
		}
	}
}
