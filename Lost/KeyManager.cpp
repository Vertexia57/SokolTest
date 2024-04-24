#include "KeyManager.h"

KeyManager::KeyManager()
{
}

void KeyManager::resetKeyData()
{
	m_KeyTappedMap.clear(); // Essentially the same as setting the value of m_KeyTappedMap to false
}

void KeyManager::feedKeyEvent(const sapp_event* user_event)
{
	sapp_keycode keyCode = user_event->key_code;
	switch (user_event->type)
	{
	case SAPP_EVENTTYPE_KEY_DOWN:
		if (!m_KeyDownMap[keyCode])
			m_KeyTappedMap[keyCode] = true;
		m_KeyDownMap[keyCode] = true;
		break;
	case SAPP_EVENTTYPE_KEY_UP:
		m_KeyDownMap[keyCode] = false;
		break;
	}
}

bool KeyManager::keyDown(sapp_keycode keyCode)
{
	if (m_KeyDownMap.count(keyCode) > 0)
		return m_KeyDownMap[keyCode];
	return false;
}

bool KeyManager::keyTapped(sapp_keycode keyCode)
{
	if (m_KeyTappedMap.count(keyCode) > 0)
		return m_KeyTappedMap[keyCode];
	return false;
}

//bool KeyManager::boundDown(sapp_keycode keyCode)
//{
//	return m_KeyDownMap[m_BindingsMap[keyCode]];
//}

namespace lost
{

	KeyManager keyManager = KeyManager();

	void feedKeyEvent(const sapp_event* user_event)
	{
		if (user_event->type == SAPP_EVENTTYPE_KEY_DOWN || user_event->type == SAPP_EVENTTYPE_KEY_UP)
			keyManager.feedKeyEvent(user_event);
	}

	void resetKeyData()
	{
		keyManager.resetKeyData();
	}

	bool keyDown(sapp_keycode keyCode)
	{
		return keyManager.keyDown(keyCode);
	}

	bool keyTapped(sapp_keycode keyCode)
	{
		return keyManager.keyTapped(keyCode);
	}

}