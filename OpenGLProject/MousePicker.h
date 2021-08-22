#pragma once

#include <iostream>
#include <glad/glad.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"

namespace fwork
{
	class MousePicker
	{
	public:
		MousePicker(){}

		glm::vec3 getPointAlongRay(Camera& cam, GLFWwindow& window, float distance)
		{
			return (getMouseRay(cam, window) * distance + cam.cameraPos);
		}

		glm::vec3 getMouseRay(Camera& cam, GLFWwindow& window);
		

		glm::vec3 getPlaneIntersection(Camera& cam, GLFWwindow& window, glm::vec3 planePoint, glm::vec3 planeNorm);

		// Fills param "intersectionRayDist" with coords of intersection with the sphere(if any exist and func returns true)
		bool getSphereIntersection(Camera& cam, GLFWwindow& window, glm::vec3 spherePos, float radius, glm::vec3& intersectionPoint);


	private:
		bool solveQuadratic(const float& a, const float& b, const float& c, float& x0, float& x1);
		
	};
}