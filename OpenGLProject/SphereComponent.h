#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>

#include "MeshComponent.h"
#include "TransformComponent.h"

namespace ecs
{
	class SphereComponent
	{
		glm::vec3 centerPos{ 0, 0, 0 };
		float radius = 1;
		TransformComponent transform_c;
	public:
		MeshComponent mesh_c;
		
		void setCenterPos(glm::vec3 newPos)
		{
			centerPos = newPos;
			transform_c.setPos(newPos);
		}

		void setRadius(float newRadius)
		{
			radius = newRadius;
			transform_c.setScale(radius, radius, radius);
		}

		glm::mat4 getTransform() { return transform_c.getTransform(); }
		float getRadius() { return radius; }
		glm::vec3 getCenterPos() { return centerPos; }
	};
}