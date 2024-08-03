#include "UIManager.h"
#include "ShaderManager.h"
#include "ImageManager.h"
#include <algorithm>

namespace lost
{
	UIManager::UIManager()
	{
	}

	UIManager::~UIManager()
	{
		for (UIWindow* window : m_UIWindows)
			delete window;
	}

	void UIManager::addUIWindow(UIWindow* windowPtr)
	{
		if (windowPtr->maxOne)
		{
			int windowOfType = FindWindowOfType(windowPtr->windowType);
			if (windowOfType != -1)
			{
				delete m_UIWindows[windowOfType];
				m_UIWindows.erase(m_UIWindows.begin() + windowOfType);
				m_UIWindows.insert(m_UIWindows.begin() + windowOfType, windowPtr);
			}
			else
			{
				m_UIWindows.push_back(windowPtr);
			}
		}
		else
		{
			m_UIWindows.push_back(windowPtr);
		}
	}

	bool UIManager::UIHasWindow(UIWindow* windowPtr)
	{
		for (int i = 0; i < m_UIWindows.size(); i++)
		{
			if (m_UIWindows[i] == windowPtr)
			{
				return true;
			}
		}
		return false;
	}

	void UIManager::removeInventoryWindows()
	{
		for (UIWindow* window : m_UIWindows)
		{
			if (window->inventoryWindow)
				window->toRemove = true;
		}
	}

	int UIManager::FindWindowOfType(std::string& type)
	{
		for (int i = 0; i < m_UIWindows.size(); i++)
		{
			if (m_UIWindows[i]->windowType == type && !m_UIWindows[i]->toRemove)
			{
				return i;
			}
		}
		return -1;
	}

	void UIManager::orderUI()
	{
		std::sort(m_UIWindows.begin(), m_UIWindows.end(), [](UIWindow* a, UIWindow* b) { return a->orderValue < b->orderValue; });
	}

	void UIManager::updateUI()
	{
		for (int i = m_UIWindows.size() - 1; i >= 0; i--)
		{
			if (m_UIWindows[i]->toRemove)
			{
				delete m_UIWindows[i];
				m_UIWindows.erase(m_UIWindows.begin() + i);
			}
		}

		if (!ImGui::IsAnyItemActive())
		{
			for (UIWindow* window : m_UIWindows)
				window->update();
		}
	}

	void UIManager::renderUI()
	{
		ShaderID oldShader = lost::shaderManager->currentShader;
		lost::resetShader();
		lost::clearImage();

		for (UIWindow* window : m_UIWindows)
		{
			if (!window->toRemove)
				window->render();
		}

		lost::bindShader(oldShader);
	}

	bool UIManager::isUISelected()
	{
		for (UIWindow* window : m_UIWindows)
		{
			if (window->selected)
				return true;
		}
		return false;
	}

	// Global function definitions

	UIManager* uiManager = nullptr;

	void createUIManager()
	{
		uiManager = new UIManager();
	}

	void destroyUIManager()
	{
		delete uiManager;
	}

	void addUIWindow(UIWindow* windowPtr)
	{
		uiManager->addUIWindow(windowPtr);
		uiManager->orderUI();
	}

	bool UIHasWindow(UIWindow* windowPtr)
	{
		return uiManager->UIHasWindow(windowPtr);
	}

	void removeInventoryWindows()
	{
		uiManager->removeInventoryWindows();
	}

	void updateUI()
	{
		uiManager->updateUI();
	}

	void renderUI()
	{
		uiManager->renderUI();
	}

	bool isUISelected()
	{
		return ImGui::IsAnyItemActive() || uiManager->isUISelected();
	}
}