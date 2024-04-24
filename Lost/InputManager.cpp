#include "InputManager.h"

InputManager::InputManager()
{
}

void InputManager::resetKeyData()
{
	m_KeyTappedMap.clear(); // Essentially the same as setting the value of m_KeyTappedMap to false

	// Resets all data in m_MouseTappedMap to false
	for (bool& mouseData : m_MouseTaps)
		mouseData = false;
	// Resets all data in m_MouseReleasedMap to false
	for (bool& mouseData : m_MouseReleases)
		mouseData = false;

	m_Scroll = 0;
}

void InputManager::feedKeyEvent(const sapp_event* user_event)
{
	sapp_keycode keyCode = user_event->key_code;
	switch (user_event->type)
	{
	case SAPP_EVENTTYPE_KEY_DOWN:
		// Set the released data
		if (!m_KeyDownMap[keyCode])
			m_KeyTappedMap[keyCode] = true;
		// Set the hold data
		m_KeyDownMap[keyCode] = true;
		break;
	case SAPP_EVENTTYPE_KEY_UP:
		// Set the hold data
		m_KeyDownMap[keyCode] = false;
		break;
	}
}

void InputManager::feedMouseEvent(const sapp_event* user_event)
{
	int mouseButton = user_event->mouse_button;
	switch (user_event->type)
	{
	case SAPP_EVENTTYPE_MOUSE_DOWN:
		// Set the released data
		if (!m_MouseDowns[mouseButton])
			m_MouseTaps[mouseButton] = true;
		// Set the hold data
		m_MouseDowns[mouseButton] = true;
		break;
	case SAPP_EVENTTYPE_MOUSE_UP:
		// Set the released data
		if (m_MouseDowns[mouseButton])
			m_MouseReleases[mouseButton] = true;
		// Set the hold data
		m_MouseDowns[mouseButton] = false;
		break;
	case SAPP_EVENTTYPE_MOUSE_SCROLL:
		m_Scroll += user_event->scroll_y;
		break;
	}

	m_MousePos = { user_event->mouse_x, user_event->mouse_y };
}

bool InputManager::keyDown(sapp_keycode keyCode)
{
	if (m_KeyDownMap.count(keyCode) > 0)
		return m_KeyDownMap[keyCode];
	return false;
}

bool InputManager::keyTapped(sapp_keycode keyCode)
{
	if (m_KeyTappedMap.count(keyCode) > 0)
		return m_KeyTappedMap[keyCode];
	return false;
}

bool InputManager::mouseDown(int mouseButton)
{
	return m_MouseDowns[mouseButton];
}

bool InputManager::mouseTapped(int mouseButton)
{
	return m_MouseTaps[mouseButton];
}

bool InputManager::mouseReleased(int mouseButton)
{
	return m_MouseReleases[mouseButton];
}

lost::Vector2D InputManager::mousePos()
{
	return m_MousePos;
}

int InputManager::mouseScroll()
{
	return m_Scroll;
}

//bool KeyManager::boundDown(sapp_keycode keyCode)
//{
//	return m_KeyDownMap[m_BindingsMap[keyCode]];
//}

namespace lost
{

	InputManager inputManager = InputManager();

	void feedInputEvent(const sapp_event* user_event)
	{
		if (user_event->type == SAPP_EVENTTYPE_KEY_DOWN || user_event->type == SAPP_EVENTTYPE_KEY_UP)
			inputManager.feedKeyEvent(user_event);
		if (user_event->type == SAPP_EVENTTYPE_MOUSE_DOWN ||
			user_event->type == SAPP_EVENTTYPE_MOUSE_UP ||
			user_event->type == SAPP_EVENTTYPE_MOUSE_MOVE ||
			user_event->type == SAPP_EVENTTYPE_MOUSE_SCROLL)
			inputManager.feedMouseEvent(user_event);
			
	}

	void resetInputData()
	{
		inputManager.resetKeyData();
	}

	bool keyDown(sapp_keycode keyCode)
	{
		return inputManager.keyDown(keyCode);
	}

	bool keyTapped(sapp_keycode keyCode)
	{
		return inputManager.keyTapped(keyCode);
	}

	bool mouseDown(int mouseButton)
	{
		return inputManager.mouseDown(mouseButton);
	}

	bool mouseTapped(int mouseButton)
	{
		return inputManager.mouseTapped(mouseButton);
	}

	bool mouseReleased(int mouseButton)
	{
		return inputManager.mouseReleased(mouseButton);
	}

	lost::Vector2D mousePos()
	{
		return inputManager.mousePos();
	}

	int mouseScroll()
	{
		return inputManager.mouseScroll();
	}

}