#pragma once

#include "Component.h"
#include "Id.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace ecs
{
	class TransformComponent : public Component
	{
		glm::mat4 transform_mat = glm::mat4(1);
	public:
		glm::mat4 translation_mat = glm::mat4(1);
		glm::mat4 rotation_mat = glm::mat4(1);
		glm::mat4 scale_mat = glm::mat4(1);

		TransformComponent()
		{
			Component::id = ecs::ComponentId::TRANSFORM;
		}

		glm::mat4 getTransform() 
		{
			transform_mat = translation_mat * rotation_mat * scale_mat;
			return transform_mat;
		}

		glm::vec3 getPos()
		{
			glm::vec4 vec = translation_mat * glm::vec4(0, 0, 0, 1);
			return glm::vec3(vec.x, vec.y, vec.z);
		}

		void move(float x, float y, float z)
		{
			translation_mat = glm::translate(translation_mat, glm::vec3(x, y, z));
		}

		void setPos(float x, float y, float z)
		{
			translation_mat = glm::translate(glm::mat4(1), glm::vec3(x, y, z));
		}

		void setPos(glm::vec3 vec)
		{
			translation_mat = glm::translate(glm::mat4(1), vec);
		}

		void rotate(float angle, float rotAxisX, float rotAxisY, float rotAxisZ)
		{
			rotation_mat = glm::rotate(rotation_mat, angle, glm::vec3(rotAxisX, rotAxisY, rotAxisZ));
		}

		void scale(float factorX, float factorY, float factorZ)
		{
			scale_mat = glm::scale(scale_mat, glm::vec3(factorX, factorY, factorZ));
		}

		void setScale(float factorX, float factorY, float factorZ)
		{
			scale_mat = glm::scale(glm::mat4(1), glm::vec3(factorX, factorY, factorZ));
		}
	};
}