#include "UIManager.h"
#include "ShaderManager.h"
#include "ImageManager.h"

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
		m_UIWindows.push_back(windowPtr);
	}

	void UIManager::destroyUIWindow(UIWindow* windowPtr)
	{
		// [?] This is slower than storing an map of the pointers to the location in the vector.
		// [?] Though that does use a lot more ram and is really a waste of time

		for (int i = 0; i < m_UIWindows.size(); i++)
		{
			if (m_UIWindows[i] = windowPtr)
			{
				if (windowPtr) // Even though the user shouldn't delete the UIWindow, just check anyways
					delete windowPtr;
				else
					fprintf(stderr, "\"delete\" was used on a UIWindow pointer somewhere outside of the UIManager, this is not necessary.\n");
				m_UIWindows.erase(m_UIWindows.begin() + i);
				break;
			}
		}
	}

	void UIManager::removeUIWindow(UIWindow* windowPtr)
	{
		// [?] This is slower than storing an map of the pointers to the location in the vector.
		// [?] Though that does use a lot more ram and is really a waste of time

		for (int i = 0; i < m_UIWindows.size(); i++)
		{
			if (m_UIWindows[i] = windowPtr)
			{
				m_UIWindows.erase(m_UIWindows.begin() + i);
				break;
			}
		}
	}

	void UIManager::updateUI()
	{
		if (!ImGui::IsAnyItemActive())
		{
			for (UIWindow* window : m_UIWindows)
				window->update();
		}
	}

	void UIManager::renderUI()
	{
		ShaderID oldShader = lost::shaderManager->currentShader;
		lost::unbindShader();
		lost::clearImage();

		for (UIWindow* window : m_UIWindows)
			window->render();

		lost::bindShader(oldShader);
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
	}

	void destroyUIWindow(UIWindow* windowPtr)
	{
		uiManager->destroyUIWindow(windowPtr);
	}

	void removeUIWindow(UIWindow* windowPtr)
	{
		uiManager->removeUIWindow(windowPtr);
	}

	void updateUI()
	{
		uiManager->updateUI();
	}

	void renderUI()
	{
		uiManager->renderUI();
	}
}