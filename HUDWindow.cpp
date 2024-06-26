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
}

HUDWindow::~HUDWindow()
{
}

void HUDWindow::update()
{
	m_Bounds = { 0.0f, 0.0f, (float)sapp_width(), (float)sapp_height() };
	selected = false;

	if (lost::keyTapped(SAPP_KEYCODE_B))
	{
		m_Building = !m_Building;
		if (m_Building)
			m_Rotation = 0;
		if (!g_PlayerPointer->lockActions && m_Building)
			g_PlayerPointer->lockActions = true;
		if (g_PlayerPointer->lockActions && !m_Building)
			g_PlayerPointer->lockActions = false;
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
		int buildingCount = g_TileManager.buildingRefs.size();
		m_HoveredBuildingSlot = (lost::mousePos().y >= m_Bounds.h - 1.0f - 76.0f) ? floor((lost::mousePos().x - (m_Bounds.w - buildingCount * 76.0f) / 2.0f) / 76.0f) : -1;
		if (m_HoveredBuildingSlot >= 0 && m_HoveredBuildingSlot < buildingCount)
		{
			selected = true;
			if (lost::mouseTapped(0))
			{
				m_SelectedBuilding = g_TileManager.buildingRefs[m_HoveredBuildingSlot];
				m_SelectedBuildingSlot = m_HoveredBuildingSlot;
				m_Rotation = 0;
			}
		}

		if (lost::keyTapped(SAPP_KEYCODE_R))
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
	lost::useImage(m_TopLeftUITexID);
	float imageWidth = lost::getImage(m_TopLeftUITexID)->width;
	float imageHeight = lost::getImage(m_TopLeftUITexID)->height;
	sgp_draw_textured_rect(0, { 0.0f, 0.0f, imageWidth * 4.0f, imageHeight * 4.0f }, { 0.0f, 0.0f, imageWidth, imageHeight });
	// Money Count
	lost::renderTextPro(std::to_string(g_PlayerPointer->moneyCount), { 12.0f * 4.0f, 29.0f * 4.0f }, 1.0f, LOST_TEXT_ALIGN_LEFT, LOST_TEXT_ALIGN_MIDDLE);

	if (m_Building)
	{
		int buildingCount = g_TileManager.buildingRefs.size();
		for (int i = 0; i < buildingCount; i++)
		{
			TileEntityStruct* buildingRef = g_TileManager.buildingRefs[i];

			int maxImageLength = std::max(lost::getImage(buildingRef->texture)->width / buildingRef->totalFrames, lost::getImage(buildingRef->texture)->height / buildingRef->totalVariants);
			float scale = 1.0f / (float)maxImageLength;
			float imageWidth = (float)lost::getImage(buildingRef->texture)->width / buildingRef->totalFrames;
			float imageHeight = (float)lost::getImage(buildingRef->texture)->height / buildingRef->totalVariants;

			lost::clearImage();
			if (m_SelectedBuildingSlot == i)
				sgp_set_color(0.7f, 0.7f, 0.7f, 0.2f);
			else if (m_HoveredBuildingSlot != i)
				sgp_set_color(0.0f, 0.0f, 0.0f, 0.2f);
			else
				sgp_set_color(0.5f, 0.5f, 0.5f, 0.2f);

			sgp_draw_filled_rect((m_Bounds.w - buildingCount * 76.0f) / 2.0f + i * 76.0f, m_Bounds.h - 76.0f, 76.0f, 76.0f);

			sgp_set_color(1.0f, 1.0f, 1.0f, 1.0f);
			lost::useImage(buildingRef->texture);
			sgp_draw_textured_rect(0,
				{ (m_Bounds.w - buildingCount * 76.0f) / 2.0f + 6.0f + i * 76.0f, m_Bounds.h - 70.0f, imageWidth * 64.0f * scale, imageHeight * 64.0f * scale },
				{ 0.0f, 0.0f, imageWidth, imageHeight }
			);

			lost::clearImage();
			lost::renderTextPro(std::to_string((int)buildingRef->cost),
				{ (m_Bounds.w - buildingCount * 76.0f + 76.0f) / 2.0f + i * 76.0f, m_Bounds.h - 74.0f }, 1.0f,
				LOST_TEXT_ALIGN_MIDDLE, LOST_TEXT_ALIGN_BOTTOM);
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
