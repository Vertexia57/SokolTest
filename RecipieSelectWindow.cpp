#include "RecipieSelectWindow.h"
#include "FactoryTileEntity.h"

RecipieSelectWindow::RecipieSelectWindow(lost::Bound2D bounds, FactoryTileEntity* factoryRef, std::string& craftingGroup)
	: BoxWindow(bounds)
{
	setName("Container");
	inventoryWindow = true;
	m_FactoryRef = factoryRef;
	m_CraftingGroup = craftingGroup;
	m_CraftingGroupRef = &g_ItemManager.getCraftingGroupData(craftingGroup);
}

RecipieSelectWindow::RecipieSelectWindow(lost::Bound2D bounds, lost::NBoxData nBoxData, int toastHeight, FactoryTileEntity* factoryRef, std::string& craftingGroup)
	: BoxWindow(bounds, nBoxData, toastHeight)
{
	setName("Container");
	inventoryWindow = true;
	m_FactoryRef = factoryRef;
	m_CraftingGroup = craftingGroup;
}

RecipieSelectWindow::~RecipieSelectWindow()
{
}

void RecipieSelectWindow::update()
{
	BoxWindow::update();

	lost::Vector2D mousePos = lost::mousePos();
	int slotX = (int)floor((float)(mousePos.x - m_Bounds.x - 10) / m_SlotSize);
	int slotY = (int)floor((float)(mousePos.y - m_Bounds.y - 10 - m_ToastHeight) / m_SlotSize);
	int slotIndex = slotX + slotY * m_ContainerWidth;

	if (slotX >= 0 && slotX < m_ContainerWidth && slotY >= 0 && slotY < m_ContainerHeight)
	{
		m_HighlightedSlot = { slotX, slotY };
		if (lost::mouseTapped(0) && slotIndex < m_CraftingGroupRef->size())
		{
			toRemove = true;
			auto it = m_CraftingGroupRef->begin();
			for (int i = 0; i < slotIndex; i++)
				it++;
			m_FactoryRef->setRecipie(it->second);
		}
	}
	else
	{
		m_HighlightedSlot = { -1, -1 };
	}
}

void RecipieSelectWindow::render()
{
	BoxWindow::render();

	if (m_FactoryRef->getInventory())
	{
		size_t index = 0;
		for (int y = 0; y < m_ContainerHeight; y++)
		{
			for (int x = 0; x < m_ContainerWidth; x++)
			{
				if (index <= m_FactoryRef->getInventory()->size - 1)
				{
					Item* item = m_FactoryRef->getInventory()->getItem(index);

					lost::NBoxData slotNBox = {};
					slotNBox.bottomSize = 2;
					slotNBox.topSize = 2;
					slotNBox.rightSize = 2;
					slotNBox.leftSize = 2;
					slotNBox.imageSize = { 5, 5 };
					slotNBox.scale = 4.0f;
					slotNBox.bounds = { m_Bounds.x + 15.0f + x * m_SlotSize, m_Bounds.y + 20.0f + m_ToastHeight + y * m_SlotSize, m_SlotSize, m_SlotSize };
					slotNBox.texture = lost::getImageID("SlotNBox");
					lost::renderNBox(slotNBox);

					lost::clearImage();
					sgp_set_color(0.0f, 1.0f, 0.0f, 1.0f);
					sgp_draw_filled_rect(m_Bounds.x + 15.0f, m_Bounds.y + 30.0f + m_ToastHeight + m_ContainerHeight * m_SlotSize, m_FactoryRef->getCompletionPercentage() * (m_Bounds.w - 30.0f), 10.0f);
					sgp_set_color(1.0f, 1.0f, 1.0f, 1.0f);

					if (!item->empty)
					{

						lost::useImage(item->textureID);
						float imageWidth = lost::getImage(item->textureID)->width / item->refStruct->frames;
						float imageHeight = lost::getImage(item->textureID)->height / item->refStruct->variants;

						sgp_rect renderArea = { m_Bounds.x + 15.0f + x * m_SlotSize, m_Bounds.y + 20.0f + m_ToastHeight + y * m_SlotSize, m_SlotSize, m_SlotSize };

						sgp_draw_textured_rect(0, renderArea, { 0, imageHeight * item->variant, imageWidth, imageHeight });
						lost::renderTextPro(
							std::to_string(item->StackSize),
							{ m_Bounds.x + x * m_SlotSize + m_SlotSize, m_Bounds.y + m_ToastHeight + y * m_SlotSize + m_SlotSize },
							0.5,
							LOST_TEXT_ALIGN_RIGHT, LOST_TEXT_ALIGN_MIDDLE
						);
					}

				}
				index++;
			}
		}

		if (m_HighlightedSlot.x != -1)
		{
			lost::clearImage();
			sgp_set_color(1.0f, 1.0f, 1.0f, 0.2f);
			sgp_draw_filled_rect(m_HighlightedSlot.x * m_SlotSize + m_Bounds.x + 15.0f, m_HighlightedSlot.y * m_SlotSize + m_Bounds.y + 20.0f + m_ToastHeight, m_SlotSize, m_SlotSize);
			sgp_set_color(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}
	else
	{
		int xSlot = 0;
		int ySlot = 0;
		int index = 0;
		std::string hoveredKey = "";
		for (auto& [key, val] : *m_CraftingGroupRef)
		{
			lost::NBoxData slotNBox = {};
			slotNBox.bottomSize = 2;
			slotNBox.topSize = 2;
			slotNBox.rightSize = 2;
			slotNBox.leftSize = 2;
			slotNBox.imageSize = { 5, 5 };
			slotNBox.scale = 4.0f;
			slotNBox.bounds = { m_Bounds.x + 15.0f + xSlot * m_SlotSize, m_Bounds.y + 20.0f + m_ToastHeight + ySlot * m_SlotSize, m_SlotSize, m_SlotSize };
			slotNBox.texture = lost::getImageID("SlotNBox");
			lost::renderNBox(slotNBox);

			TextureID icon = val->icon;
			if (icon == -1)
				icon = g_ItemManager.getItemData(val->results[0].id)->textureID;

			lost::useImage(icon);
			float imageWidth = lost::getImage(icon)->width;
			float imageHeight = lost::getImage(icon)->height;

			sgp_rect renderArea = { m_Bounds.x + 15.0f + xSlot * m_SlotSize, m_Bounds.y + 20.0f + m_ToastHeight + ySlot * m_SlotSize, m_SlotSize, m_SlotSize };

			sgp_draw_textured_rect(0, renderArea, { 0, 0, imageWidth, imageHeight });

			if (xSlot == m_HighlightedSlot.x && ySlot == m_HighlightedSlot.y)
				hoveredKey = key;

			xSlot++;
			if (xSlot >= 5)
			{
				xSlot = 0;
				ySlot++;
			}

			index++;
		}

		if (m_HighlightedSlot.x != -1 && m_HighlightedSlot.x + m_HighlightedSlot.y * m_ContainerWidth <= m_CraftingGroupRef->size() - 1)
		{
			lost::clearImage();
			sgp_set_color(1.0f, 1.0f, 1.0f, 0.2f);
			sgp_draw_filled_rect(m_HighlightedSlot.x * m_SlotSize + m_Bounds.x + 15.0f, m_HighlightedSlot.y * m_SlotSize + m_Bounds.y + 20.0f + m_ToastHeight, m_SlotSize, m_SlotSize);
			sgp_set_color(1.0f, 1.0f, 1.0f, 1.0f);

			lost::Bound2D precalcTextBounds = { lost::mousePos().x, lost::mousePos().y, 0.0f, 0.0f };

			lost::Vector2D preTextPos = { 0.0f, 0.0f };
			precalcTextBounds.w = fmaxf(precalcTextBounds.w, lost::textWidth((*m_CraftingGroupRef).at(hoveredKey)->name, 0.5, -1) + 20);
			preTextPos.y += lost::textHeight((*m_CraftingGroupRef).at(hoveredKey)->name, 0.5, -1) + 10;

			std::string timeText = std::to_string((*m_CraftingGroupRef).at(hoveredKey)->timeToCraft);
			timeText.erase(timeText.find_last_not_of('0') + 1, std::string::npos);
			timeText.erase(timeText.find_last_not_of('.') + 1, std::string::npos);
			std::string timeLocalText = std::to_string((*m_CraftingGroupRef).at(hoveredKey)->timeToCraft / m_FactoryRef->getFactorySpeed());
			timeLocalText.erase(timeLocalText.find_last_not_of('0') + 1, std::string::npos);
			timeLocalText.erase(timeLocalText.find_last_not_of('.') + 1, std::string::npos);

			precalcTextBounds.w = fmaxf(precalcTextBounds.w, lost::textWidth("Time to Craft: " + timeText + " seconds", 0.35, -1) + 20);
			preTextPos.y += lost::textHeight("Time to Craft: " + timeText + " seconds", 0.35, -1) + 8;
			precalcTextBounds.w = fmaxf(precalcTextBounds.w, lost::textWidth("In this machine: " + timeLocalText + " seconds", 0.35, -1) + 20);
			preTextPos.y += lost::textHeight("In this machine: " + timeLocalText + " seconds", 0.35, -1) + 8;
			preTextPos.y += lost::textHeight("Ingredients:", 0.5, -1) + 10;
			preTextPos.y += m_SlotSize;
			preTextPos.y += lost::textHeight("Results:", 0.5, -1) + 10;
			preTextPos.y += m_SlotSize;

			precalcTextBounds.h = preTextPos.y + 20.0f;

			lost::NBoxData hoverNBox = {};
			hoverNBox.bottomSize = 2;
			hoverNBox.topSize = 2;
			hoverNBox.rightSize = 2;
			hoverNBox.leftSize = 2;
			hoverNBox.imageSize = { 5, 5 };
			hoverNBox.scale = 4.0f;
			hoverNBox.bounds = precalcTextBounds;
			hoverNBox.texture = lost::getImageID("HoverNBox");
			lost::renderNBox(hoverNBox);

			lost::Vector2D textPos = { lost::mousePos().x + 10, lost::mousePos().y + 5 };
			lost::renderTextPro((*m_CraftingGroupRef).at(hoveredKey)->name, textPos, 0.5, LOST_TEXT_ALIGN_LEFT, LOST_TEXT_ALIGN_TOP);

			textPos.y += lost::textHeight((*m_CraftingGroupRef).at(hoveredKey)->name, 0.5, -1) + 10;
			sgp_set_color(0.8f, 0.8f, 0.8f, 1.0f);
			lost::renderTextPro("Time to Craft: " + timeText + " seconds", textPos, 0.35, LOST_TEXT_ALIGN_LEFT, LOST_TEXT_ALIGN_TOP);
			textPos.y += lost::textHeight("Time to Craft: " + timeText + " seconds", 0.35, -1) + 8;

			lost::renderTextPro("In this machine: " + timeLocalText + " seconds", textPos, 0.35, LOST_TEXT_ALIGN_LEFT, LOST_TEXT_ALIGN_TOP);
			textPos.y += lost::textHeight("In this machine: " + timeLocalText + " seconds", 0.35, -1) + 8;

			sgp_set_color(0.9f, 0.9f, 0.9f, 0.9f);
			lost::renderTextPro("Ingredients:", textPos, 0.4, LOST_TEXT_ALIGN_LEFT, LOST_TEXT_ALIGN_TOP);
			textPos.y += lost::textHeight("Ingredients:", 0.4, -1) + 10;

			sgp_set_color(1.0f, 1.0f, 1.0f, 1.0f);

			int ingredientIndex = 0;
			for (RecipieRefStruct::IdCountPair& pair : (*m_CraftingGroupRef).at(hoveredKey)->ingredients)
			{
				lost::NBoxData slotNBox = {};
				slotNBox.bottomSize = 2;
				slotNBox.topSize = 2;
				slotNBox.rightSize = 2;
				slotNBox.leftSize = 2;
				slotNBox.imageSize = { 5, 5 };
				slotNBox.scale = 4.0f;
				slotNBox.bounds = { textPos.x + m_SlotSize * ingredientIndex, textPos.y, m_SlotSize, m_SlotSize };
				slotNBox.texture = lost::getImageID("HoverNBox");
				lost::renderNBox(slotNBox);

				ItemRefStruct* itemRef = g_ItemManager.getItemData(pair.id);

				lost::useImage(itemRef->textureID);
				float imageWidth = lost::getImage(itemRef->textureID)->width / itemRef->frames;
				float imageHeight = lost::getImage(itemRef->textureID)->height / itemRef->variants;

				sgp_rect renderArea = { textPos.x + m_SlotSize * ingredientIndex, textPos.y, m_SlotSize, m_SlotSize };

				sgp_draw_textured_rect(0, renderArea, { 0, 0, imageWidth, imageHeight });
				lost::renderTextPro(
					std::to_string(pair.count),
					{ textPos.x + m_SlotSize * (ingredientIndex + 1) - 10, textPos.y + m_SlotSize - 10 },
					0.5,
					LOST_TEXT_ALIGN_RIGHT, LOST_TEXT_ALIGN_BOTTOM
				);

				ingredientIndex++;
			}

			textPos.y += m_SlotSize + 5;
			sgp_set_color(0.9f, 0.9f, 0.9f, 0.9f);
			lost::renderTextPro("Results:", textPos, 0.4, LOST_TEXT_ALIGN_LEFT, LOST_TEXT_ALIGN_TOP);
			textPos.y += lost::textHeight("Results:", 0.4, -1) + 10;

			int resultsIndex = 0;
			for (RecipieRefStruct::IdCountPair& pair : (*m_CraftingGroupRef).at(hoveredKey)->results)
			{
				lost::NBoxData slotNBox = {};
				slotNBox.bottomSize = 2;
				slotNBox.topSize = 2;
				slotNBox.rightSize = 2;
				slotNBox.leftSize = 2;
				slotNBox.imageSize = { 5, 5 };
				slotNBox.scale = 4.0f;
				slotNBox.bounds = { textPos.x + m_SlotSize * resultsIndex, textPos.y, m_SlotSize, m_SlotSize };
				slotNBox.texture = lost::getImageID("HoverNBox");
				lost::renderNBox(slotNBox);

				ItemRefStruct* itemRef = g_ItemManager.getItemData(pair.id);

				lost::useImage(itemRef->textureID);
				float imageWidth = lost::getImage(itemRef->textureID)->width / itemRef->frames;
				float imageHeight = lost::getImage(itemRef->textureID)->height / itemRef->variants;

				sgp_rect renderArea = { textPos.x + m_SlotSize * resultsIndex, textPos.y, m_SlotSize, m_SlotSize };

				sgp_draw_textured_rect(0, renderArea, { 0, 0, imageWidth, imageHeight });
				lost::renderTextPro(
					std::to_string(pair.count),
					{ textPos.x + m_SlotSize * (resultsIndex + 1) - 10, textPos.y + m_SlotSize - 10 },
					0.5,
					LOST_TEXT_ALIGN_RIGHT, LOST_TEXT_ALIGN_BOTTOM
				);

				resultsIndex++;
			}
		}
	}
}
