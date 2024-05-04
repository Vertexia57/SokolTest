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

		/// <summary>
		/// <para>The UI Manager handles the memory of the window given, delete does not need to be ran on the window given</para>
		/// <para>The manager will do that automatically</para>
		/// </summary>
		/// <param name="windowPtr"></param>
		void addUIWindow(UIWindow* windowPtr);
		/// <summary>
		/// <para>This will deallocate/delete the window from the heap and remove it from the UIManager</para>
		/// <para>You do not need to run delete on the pointer, this will do it for you</para>
		/// </summary>
		/// <param name="windowPtr"></param>
		void destroyUIWindow(UIWindow* windowPtr);
		/// <summary>
		/// <para>This will remove the window from the manager, this function does not delete the UIWindow from the heap</para>
		/// <para>Only use this if you need to use that window later</para>
		/// </summary>
		/// <param name="windowPtr"></param>
		void removeUIWindow(UIWindow* windowPtr);

		void updateUI();
		void renderUI();

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
	/// <summary>
	/// <para>This will deallocate/delete the window from the heap and remove it from the UIManager</para>
	/// <para>You do not need to run delete on the pointer, this will do it for you</para>
	/// </summary>
	/// <param name="windowPtr"></param>
	extern void destroyUIWindow(UIWindow* windowPtr);
	/// <summary>
	/// <para>This will remove the window from the manager, this function does NOT delete the UIWindow from the heap</para>
	/// <para>Only use this if you need to use that window later</para>
	/// </summary>
	/// <param name="windowPtr"></param>
	extern void removeUIWindow(UIWindow* windowPtr);

	// Updates the UI, allowing for them to react to user inputs
	extern void updateUI();
	// Renders the UI to the screen all rendering at the same time
	extern void renderUI();

}