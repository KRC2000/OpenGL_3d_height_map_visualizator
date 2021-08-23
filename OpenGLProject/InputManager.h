#pragma once

#include <iostream>
#include <vector>
#include <glfw3.h>

namespace fwork
{
	class InputManager
	{
	public:
		enum class KeyType
		{
			MOUSE_LEFT,
			MOUSE_RIGHT,
			MOUSE_MIDLE,
			AMOUNT
		};
	private:
		struct Key
		{
			KeyType keyType;
			bool isDown = false;
			bool pressed = false;
			bool released = false;
		};
		
		static std::vector<Key> keys;
		
	public:
		InputManager();

		// Call once before game cycle and input usage
		static void generateKeys();

		static Key getKey(KeyType keyType);

		static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
		
		// Call this in loop afer all input usage
		static void update();
		

	};

}
