#include "RecipieSelectWindow.h"
#include "FactoryTileEntity.h"
#include "Player.h"

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

	if (!m_FactoryRef->hasInventory())
	{
		Container* refContainer = m_FactoryRef->getInventory();
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
	else
	{
		// Use base container logic

		if (slotIndex == m_FactoryRef->getRecipie()->ingredients.size())
		{
			slotIndex = -1;
			slotX = -1;
			slotY = -1;
		}
		else if (slotIndex > m_FactoryRef->getRecipie()->ingredients.size())
		{
			slotIndex--;
		}

		Container* refContainer = m_FactoryRef->getInventory();
		if (slotX >= 0 && slotX < m_ContainerWidth && slotY >= 0 && slotY < m_ContainerHeight && slotIndex < refContainer->size)
		{
			m_HighlightedSlot = { slotX, slotY };

			// Left click
			if (lost::mouseTapped(0))
			{
				Item* itemHovered = refContainer->getItem(slotIndex);
				if ((itemHovered->empty || g_PlayerPointer->holdingItem.empty || !refContainer->isSlotSameType(slotIndex, g_PlayerPointer->holdingItem)) && !itemHovered->locked)
				{
					// Slot wasn't accessable, swap
					SwapItems(itemHovered, &g_PlayerPointer->holdingItem);
				}
				else if ((g_PlayerPointer->holdingItem.empty || g_PlayerPointer->holdingItem.itemID == itemHovered->itemID) && itemHovered->locked && itemHovered->StackSize > 0)
				{
					// Player hand is empty or contains the item that matches the locked slot

					if (g_PlayerPointer->holdingItem.empty) // Any
					{
						// Fill the player hand with the locked slot's item and set the stack size of the slot to 0
						g_PlayerPointer->holdingItem = *itemHovered;
						g_PlayerPointer->holdingItem.locked = false;
						itemHovered->StackSize = 0;
					}
					else if (!itemHovered->output) // Input & Locked
					{
						// Same as if it wasn't locked, fill slot with as much as it can, from the player hand
						int amountToFill = (itemHovered->StackSize + g_PlayerPointer->holdingItem.StackSize > itemHovered->maxStackSize) ? itemHovered->maxStackSize - itemHovered->StackSize : g_PlayerPointer->holdingItem.StackSize;
						itemHovered->StackSize += amountToFill;
						g_PlayerPointer->holdingItem.StackSize -= amountToFill;
						if (g_PlayerPointer->holdingItem.StackSize <= 0)
							g_PlayerPointer->holdingItem = Item();
					}
					else // Output & Locked
					{
						// Fill player hand with an much as it can from the output
						int amountToFill = (itemHovered->StackSize + g_PlayerPointer->holdingItem.StackSize > itemHovered->maxStackSize) ? g_PlayerPointer->holdingItem.maxStackSize - g_PlayerPointer->holdingItem.StackSize : itemHovered->StackSize;
						itemHovered->StackSize -= amountToFill;
						g_PlayerPointer->holdingItem.StackSize += amountToFill;
					}
				}
				else
				{
					if (refContainer->isSlotAccessable(slotIndex, g_PlayerPointer->holdingItem))
					{
						// Slot was accessable, add the held item stack to the item stack in that location
						// Account for overflow
						int amountToFill = (itemHovered->StackSize + g_PlayerPointer->holdingItem.StackSize > itemHovered->maxStackSize) ? itemHovered->maxStackSize - itemHovered->StackSize : g_PlayerPointer->holdingItem.StackSize;
						itemHovered->StackSize += amountToFill;
						g_PlayerPointer->holdingItem.StackSize -= amountToFill;
						if (g_PlayerPointer->holdingItem.StackSize <= 0)
							g_PlayerPointer->holdingItem = Item();
					}
				}
				m_FactoryRef->checkRecipie();
			}

			// Right Click
			if (lost::mouseTapped(1))
			{
				Item* itemHovered = refContainer->getItem(slotIndex);
				if (g_PlayerPointer->holdingItem.empty)
				{
					// Player was not holding an item, half the stack
					if (itemHovered->StackSize > 1)
					{
						int total = itemHovered->StackSize;
						g_PlayerPointer->holdingItem = *itemHovered;
						g_PlayerPointer->holdingItem.locked = false; // In case the item that was hovered was locked
						g_PlayerPointer->holdingItem.StackSize = (int)ceil((float)total / 2.0f);
						total -= (int)ceil((float)total / 2.0f);
						itemHovered->StackSize = total;
					}
					else if (itemHovered->StackSize > 0)
					{
						// Slot was just a stack of one, swap the empty hand and the slot
						if (!itemHovered->locked)
						{
							SwapItems(itemHovered, &g_PlayerPointer->holdingItem);
						}
						else // Locked
						{
							g_PlayerPointer->holdingItem = *itemHovered;
							g_PlayerPointer->holdingItem.locked = false;
							itemHovered->StackSize = 0;
						}
					}
					m_FactoryRef->checkRecipie();
				}
				else if (refContainer->isSlotAccessable(slotIndex, g_PlayerPointer->holdingItem))
				{
					// Player is holding an item and that slot is accessable

					if (itemHovered->empty)
					{
						// The slot was empty
						*itemHovered = g_PlayerPointer->holdingItem;
						itemHovered->StackSize = 1;
					}
					else
					{
						// The slot wasn't empty but was accessable
						itemHovered->StackSize++;
					}

					g_PlayerPointer->holdingItem.StackSize--;
					if (g_PlayerPointer->holdingItem.StackSize <= 0)
						g_PlayerPointer->holdingItem = Item();
					m_FactoryRef->checkRecipie();
				}
			}
		}
		else
		{
			// Hovered slot was outside of actual slot range
			m_HighlightedSlot = { -1, -1 };

			if (slotIndex == refContainer->size && slotX >= 0 && slotX < m_ContainerWidth && slotY >= 0 && slotY < m_ContainerHeight)
			{
				m_HighlightedSlot = { slotIndex + 1, 0 };
				if (lost::mouseTapped(0))
					m_FactoryRef->setRecipie(nullptr);
			}
		}
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
				size_t slotIndex = index;
				int slotX = x;
				int slotY = y;

				if (index == m_FactoryRef->getRecipie()->ingredients.size())
				{
					slotIndex = -1;
					slotX = -1;
					slotY = -1;
				}
				else if (index > m_FactoryRef->getRecipie()->ingredients.size())
				{
					slotIndex--;
				}

				if (slotIndex <= m_FactoryRef->getInventory()->size - 1)
				{
					Item* item = m_FactoryRef->getInventory()->getItem(slotIndex);

					lost::NBoxData slotNBox = {};
					slotNBox.bottomSize = 2;
					slotNBox.topSize = 2;
					slotNBox.rightSize = 2;
					slotNBox.leftSize = 2;
					slotNBox.imageSize = { 5, 5 };
					slotNBox.scale = 4.0f;
					slotNBox.bounds = { m_Bounds.x + 15.0f + slotX * m_SlotSize, m_Bounds.y + 20.0f + m_ToastHeight + slotY * m_SlotSize, m_SlotSize, m_SlotSize };
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

						sgp_rect renderArea = { m_Bounds.x + 15.0f + slotX * m_SlotSize, m_Bounds.y + 20.0f + m_ToastHeight + slotY * m_SlotSize, m_SlotSize, m_SlotSize };

						sgp_draw_textured_rect(0, renderArea, { 0, imageHeight * item->variant, imageWidth, imageHeight });
						lost::renderTextPro(
							std::to_string(item->StackSize),
							{ m_Bounds.x + slotX * m_SlotSize + m_SlotSize, m_Bounds.y + m_ToastHeight + slotY * m_SlotSize + m_SlotSize },
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
			for (IdCountPair& pair : (*m_CraftingGroupRef).at(hoveredKey)->ingredients)
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
			sgp_set_color(1.0f, 1.0f, 1.0f, 1.0f);

			int resultsIndex = 0;
			for (IdCountPair& pair : (*m_CraftingGroupRef).at(hoveredKey)->results)
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
