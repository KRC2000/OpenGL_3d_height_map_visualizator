#pragma once

#include "ObjectBP.h"

#include <glm/glm.hpp>


#include "InputManager.h"
#include "MousePicker.h"

#include "SphereComponent.h"


class BarrelBP: public ObjectBP
{
public:
	ecs::SphereComponent sphere_c;

	void update()
	{
		if (fwork::InputManager::getKey(fwork::InputManager::KeyType::MOUSE_LEFT).pressed)
		{
			if (fwork::MousePicker::getSphereIntersection(sphere_c.getCenterPos(), sphere_c.getRadius()))
				sphere_c.mesh_c.setColor(glm::vec3(1, 0, 0));
		}
	}
};