#pragma once

#include "Component.h"
#include "Id.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace ecs
{
	struct TransformComponent : public Component
	{
		glm::mat4 transform = glm::mat4(1);

		TransformComponent()
		{
			Component::id = ecs::ComponentId::TRANSFORM;
		}

		void move(float x, float y, float z)
		{
			transform = glm::translate(transform, glm::vec3(x, y, z));
		}

		void rotate(float angle, float rotAxisX, float rotAxisY, float rotAxisZ)
		{
			transform = glm::rotate(transform, angle, glm::vec3(rotAxisX, rotAxisY, rotAxisZ));
		}

		void scale(float factorX, float factorY, float factorZ)
		{
			transform = glm::scale(transform, glm::vec3(factorX, factorY, factorZ));
		}
	};
}