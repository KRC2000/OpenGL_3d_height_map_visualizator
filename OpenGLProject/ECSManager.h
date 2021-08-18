#pragma once

#include <vector>

#include "Component.h"
#include "Id.h"

namespace ecs
{
	class ECSManager
	{
		std::vector<Component*> components;
		//std::vector<System*> systems;

	public:
		Component& newComponent(Component* c)
		{
			components.push_back(c);
			return *c;
		}
	};
}