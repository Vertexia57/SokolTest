#pragma once
#include "../SokolReference.h"
#include <map>

class KeyManager
{
public:
	KeyManager();

	// Called once a frame, resets any key taps that are more than a frame old
	void resetKeyData();

	// Retrieves any key event and processes it, this can be called more than once a frame
	void feedKeyEvent(const sapp_event* user_event);

	// Returns if the key with keyCode is held down
	bool keyDown(sapp_keycode keyCode);
	// Returns if the key with keyCode is tapped
	bool keyTapped(sapp_keycode keyCode);

	// [!] TODO: Create Bindings
	//bool boundDown(sapp_keycode keyCode);

	//void createBinding();

private:
	std::map<sapp_keycode, bool> m_KeyDownMap;
	std::map<sapp_keycode, bool> m_KeyTappedMap;
	//std::map<sapp_keycode, sapp_keycode> m_BindingsMap;

};

namespace lost
{

	extern KeyManager keyManager;
	
	extern void feedKeyEvent(const sapp_event* user_event);

	// Resets the key manager's tapped data, required to be ran every frame for keyTapped to work
	extern void resetKeyData();

	// Check if key with sapp_keycode is pressed
	extern bool keyDown(sapp_keycode keyCode);
	// Check if key with sapp_keycode is tapped only returning on the KEYDOWN event
	extern bool keyTapped(sapp_keycode keyCode);

}