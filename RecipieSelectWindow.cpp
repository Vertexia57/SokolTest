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

			xSlot++;
			if (xSlot >= 5)
			{
				xSlot = 0;
				ySlot++;
			}

			index++;
		}
	}
}
