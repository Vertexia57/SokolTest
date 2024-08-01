#include "StorageWindow.h"
#include "Player.h"

StorageWindow::StorageWindow(lost::Bound2D bounds)
	: BoxWindow(bounds)
{
	setName("Container");
	inventoryWindow = true;
}

StorageWindow::StorageWindow(lost::Bound2D bounds, lost::NBoxData nBoxData, int toastHeight)
	: BoxWindow(bounds, nBoxData, toastHeight)
{
}

StorageWindow::~StorageWindow()
{
}

void StorageWindow::update()
{
	BoxWindow::update();

	lost::Vector2D mousePos = lost::mousePos();
	int slotX = (int)floor((float)(mousePos.x - m_Bounds.x - 10) / m_SlotSize);
	int slotY = (int)floor((float)(mousePos.y - m_Bounds.y - 10 - m_ToastHeight) / m_SlotSize);
	int slotIndex = slotX + slotY * m_ContainerWidth;

	if (slotX >= 0 && slotX < m_ContainerWidth && slotY >= 0 && slotY < m_ContainerHeight)
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
			}
		}
	}
	else
	{
		// Hovered slot was outside of actual slot range
		m_HighlightedSlot = { -1, -1 };
	}

}

void StorageWindow::render()
{
	BoxWindow::render();

	size_t index = m_SlotMin;
	for (int y = 0; y < m_ContainerHeight; y++)
	{
		for (int x = 0; x < m_ContainerWidth; x++)
		{
			if (index <= m_SlotMax)
			{
				Item* item = refContainer->getItem(index);

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

void StorageWindow::bindContainer(Container* refContainer_, int width, int height, int slotMin, int slotMax)
{
	refContainer = refContainer_;
	m_ContainerWidth = width;
	m_ContainerHeight = height;
	m_SlotMin = slotMin;
	m_SlotMax = slotMax;

	m_Bounds = { m_Bounds.x, m_Bounds.y, 30.0f + m_ContainerWidth * m_SlotSize, 35.0f + m_ContainerHeight * m_SlotSize + m_ToastHeight };
}
