#include "HUDWindow.h"
#include "Player.h"
#include "Lost/FontManager.h"
#include "World.h"
#include "Lost/UIManager.h"

HUDWindow::HUDWindow(lost::Bound2D bounds)
	: UIWindow(bounds)
{
	m_TopLeftUITexID = lost::getImageID("HUDTopLeft");
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
		if (!g_PlayerPointer->lockActions && m_Building)
			g_PlayerPointer->lockActions = true;
		if (g_PlayerPointer->lockActions && !m_Building)
			g_PlayerPointer->lockActions = false;
	}

	if (m_Building)
	{
		int buildingCount = g_TileManager.buildingRefs.size();
		m_HoveredBuildingSlot = (lost::mousePos().y >= m_Bounds.h - 1.0f - 76.0f) ? floor((lost::mousePos().x - (m_Bounds.w - buildingCount * 76.0f) / 2.0f) / 76.0f) : -1;
		if (m_HoveredBuildingSlot >= 0)
		{
			selected = true;
			if (lost::mouseTapped(0))
			{
				m_SelectedBuilding = g_TileManager.buildingRefs[m_HoveredBuildingSlot];
				m_SelectedBuildingSlot = m_HoveredBuildingSlot;
			}
		}

		lost::Vector2D worldMouse = lost::globalCamera.screenToWorld(lost::mousePos());
		lost::Vector2D blockMouse = { floor(worldMouse.x / 32.0f), floor(worldMouse.y / 32.0f) };

		Tile* tileHovered = g_World->getTileAt(floor(worldMouse.x / 32.0f), floor(worldMouse.y / 32.0f));
		if (tileHovered)
		{
			if (!lost::isUISelected() && m_SelectedBuilding)
			{
				lost::useImage(m_SelectedBuilding->texture);
				float imageWidth = lost::getImage(m_SelectedBuilding->texture)->width;
				float imageHeight = lost::getImage(m_SelectedBuilding->texture)->height;

				if (m_CanPlaceBuilding)
					sgp_set_color(1.0f, 1.0f, 1.0f, 0.2f);
				else
					sgp_set_color(1.0f, 0.1f, 0.1f, 0.2f);

				sgp_draw_textured_rect(0, { (blockMouse.x + m_SelectedBuilding->placementOffsetX) * 32.0f, (blockMouse.y + m_SelectedBuilding->placementOffsetY) * 32.0f, m_SelectedBuilding->width * 32.0f, m_SelectedBuilding->height * 32.0f }, { 0, 0, imageWidth, imageHeight });
				lost::clearImage();

				m_CanPlaceBuilding = (!m_SelectedBuilding->requiresSupport || g_World->checkStable(
					{ blockMouse.x + m_SelectedBuilding->placementOffsetX, blockMouse.y + 1, m_SelectedBuilding->width, 1.0f}
				)) && (g_World->checkCanPlace(
					{blockMouse.x + m_SelectedBuilding->placementOffsetX, blockMouse.y + m_SelectedBuilding->placementOffsetY, m_SelectedBuilding->width, m_SelectedBuilding->height }, m_SelectedBuilding->fillsLayers
				));
				if (lost::mouseDown(0) && m_CanPlaceBuilding && g_PlayerPointer->moneyCount >= m_SelectedBuilding->cost)
				{
					g_World->addTileEntity(new TileEntity(m_SelectedBuilding), floor(blockMouse.x + m_SelectedBuilding->placementOffsetX), floor(blockMouse.y + m_SelectedBuilding->placementOffsetY));
					g_PlayerPointer->moneyCount -= m_SelectedBuilding->cost;
				}
			}

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

			int maxImageLength = std::max(lost::getImage(buildingRef->texture)->width, lost::getImage(buildingRef->texture)->height);
			float scale = 1.0f / (float)maxImageLength;
			float imageWidth = (float)lost::getImage(buildingRef->texture)->width * scale;
			float imageHeight = (float)lost::getImage(buildingRef->texture)->height * scale;

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
				{ (m_Bounds.w - buildingCount * 76.0f) / 2.0f + 6.0f + i * 76.0f, m_Bounds.h - 70.0f, imageWidth * 64.0f, imageHeight * 64.0f },
				{ 0.0f, 0.0f, (float)lost::getImage(buildingRef->texture)->width, (float)lost::getImage(buildingRef->texture)->height }
			);

			lost::clearImage();
			lost::renderTextPro(std::to_string((int)buildingRef->cost),
				{ (m_Bounds.w - buildingCount * 76.0f + 76.0f) / 2.0f + i * 76.0f, m_Bounds.h - 74.0f }, 1.0f,
				LOST_TEXT_ALIGN_MIDDLE, LOST_TEXT_ALIGN_BOTTOM);
		}
	}
}
