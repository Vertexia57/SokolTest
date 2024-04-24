#pragma once
#include "../SokolReference.h"
#include <map>
#include "Transform2D.h"

class InputManager
{
public:
	InputManager();

	// Called once a frame, resets any key taps that are more than a frame old
	void resetKeyData();

	// Retrieves any key event and processes it, this can be called more than once a frame
	void feedKeyEvent(const sapp_event* user_event);
	// Retrieves any mouse event and processes it, this can be called more than once a frame
	void feedMouseEvent(const sapp_event* user_event);

	// Returns if the key with keyCode is held down
	bool keyDown(sapp_keycode keyCode);
	// Returns if the key with keyCode is tapped
	bool keyTapped(sapp_keycode keyCode);

	// Check if mouse button with sapp_mousebutton is pressed
	bool mouseDown(int mouseButton);
	// Check if mouse button with sapp_mousebutton is tapped only returning on the MOUSEDOWN event
	bool mouseTapped(int mouseButton);
	// Check if mouse button with sapp_mousebutton is tapped only returning on the MOUSEUP event
	bool mouseReleased(int mouseButton);

	lost::Vector2D mousePos();
	int mouseScroll();

	// [!] TODO: Create Bindings

private:

	lost::Vector2D m_MousePos = { 0, 0 };
	int m_Scroll = 0;

	std::map<sapp_keycode, bool> m_KeyDownMap;
	std::map<sapp_keycode, bool> m_KeyTappedMap;

	bool m_MouseDowns[3] = {};
	bool m_MouseTaps[3] = {};
	bool m_MouseReleases[3] = {};

};

namespace lost
{

	extern InputManager inputManager;
	
	extern void feedInputEvent(const sapp_event* user_event);

	// Resets the input manager's tapped data, required to be ran every frame for keyTapped and mouseTapped to work
	extern void resetInputData();

	// Check if key with sapp_keycode is pressed
	extern bool keyDown(sapp_keycode keyCode);
	// Check if key with sapp_keycode is tapped only returning on the KEYDOWN event
	extern bool keyTapped(sapp_keycode keyCode);

	// Check if mouse button with sapp_mousebutton is pressed
	extern bool mouseDown(int mouseButton);
	// Check if mouse button with sapp_mousebutton is tapped only returning on the MOUSEDOWN event
	extern bool mouseTapped(int mouseButton);
	// Check if mouse button with sapp_mousebutton is tapped only returning on the MOUSEUP event
	extern bool mouseReleased(int mouseButton);

	// Returns the position of the mouse
	lost::Vector2D mousePos();
	// Returns the amount the scroll wheel has moved
	int mouseScroll();

}