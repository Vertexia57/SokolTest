#pragma once
#include <vector>
#include "UIWindow.h"

namespace lost
{

	class UIManager
	{
	public:
		UIManager();
		~UIManager();

		// Adds the UIWindow to the manager
		void addUIWindow(UIWindow* windowPtr);
		// Returns if the screen currently has a window with that pointer in it
		bool UIHasWindow(UIWindow* windowPtr);
		// Removes all windows with the inventoryWindow bool set to true
		void removeInventoryWindows();
		// Returns if the screen currently has a window with that pointer in it
		int FindWindowOfType(std::string& type);

		void orderUI();

		void updateUI();
		void renderUI();

		bool isUISelected();
	private:
		std::vector<UIWindow*> m_UIWindows;
	};

	extern UIManager* uiManager;

	// Creates the UIManager, this should not be ran by the user
	extern void createUIManager();
	// Destroys the UIMangaer, this should not be ran by the user
	extern void destroyUIManager();

	/// <summary>
	/// <para>The UI Manager handles the memory of the window given, delete does not need to be ran on the window given</para>
	/// <para>The manager will do that automatically</para>
	/// </summary>
	/// <param name="windowPtr"></param>
	extern void addUIWindow(UIWindow* windowPtr);
	// Returns if the screen currently has a window with that pointer in it
	extern bool UIHasWindow(UIWindow* windowPtr);

	// Removes all windows with the inventoryWindow bool set to true
	extern void removeInventoryWindows();

	// Updates the UI, allowing for them to react to user inputs
	extern void updateUI();
	// Renders the UI to the screen all rendering at the same time
	extern void renderUI();

	// Returns if any UI is currently selected/active
	extern bool isUISelected();

}