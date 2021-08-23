#include "InputManager.h"

std::vector<fwork::InputManager::Key> fwork::InputManager::keys;

fwork::InputManager::InputManager()
{
}

void fwork::InputManager::generateKeys()
{
	for (int i = 0; i < (int)KeyType::AMOUNT; i++)
	{
		Key key;
		key.keyType = (KeyType)i;

		keys.push_back(key);
	}
}

fwork::InputManager::Key fwork::InputManager::getKey(KeyType keyType)
{
	for (Key& key : keys)
	{
		if (keyType == key.keyType) return key;
	}
}

void fwork::InputManager::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		for (Key& key : keys)
		{
			if (KeyType::MOUSE_LEFT == key.keyType)
			{
				key.isDown = true;
				key.pressed = true;
			}
		}
	}
}

void fwork::InputManager::update()
{
	for (Key& key : keys)
	{
		key.pressed = false;
		key.released = false;
	}
}
