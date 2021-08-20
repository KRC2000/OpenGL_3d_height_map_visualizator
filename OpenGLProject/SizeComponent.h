#pragma once

#include <iostream>
#include <vector>

#include <glm/glm.hpp>

#include "Component.h"
#include "Id.h"

#include "TransformComponent.h"
#include "Vec3.h"

namespace ecs
{
	class SizeComponent: public Component
	{
		glm::vec3 size;
	public:
		SizeComponent() { Component::id = ecs::ComponentId::SIZE; }

		void recalculateSize(const std::vector<float>& vertices, TransformComponent& trans_c, unsigned int offset = 0);
		glm::vec3 getSize() { return size; }
	};
}