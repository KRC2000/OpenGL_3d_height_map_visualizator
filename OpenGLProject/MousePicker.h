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
		static fwork::Camera* camPtr;
		static GLFWwindow* windowPtr;
	public:
		MousePicker(){}

		static void bindWindowCamera(GLFWwindow& window, Camera& cam);

		static glm::vec3 getPointAlongRay(Camera& cam, GLFWwindow& window, float distance);

		static glm::vec3 getMouseRay(Camera& cam, GLFWwindow& window);

		static glm::vec3 getPlaneIntersection(Camera& cam, GLFWwindow& window, glm::vec3 planePoint, glm::vec3 planeNorm);

		// Fills param "intersectionRayDist" with coords of intersection with the sphere(if any exist and func returns true)
		static bool getSphereIntersection(Camera& cam, GLFWwindow& window, glm::vec3 spherePos, float radius, glm::vec3& intersectionPoint);

		// Uses binded through "bindWindowCamera()" window and camera references
		// Fills param "intersectionRayDist" with coords of intersection with the sphere(if any exist and func returns true)
		static bool getSphereIntersection(glm::vec3 spherePos, float radius, glm::vec3& intersectionPoint);

		// Uses binded through "bindWindowCamera()" window and camera references
		// Only returns true or false, use do decide if intersected or not
		static bool getSphereIntersection(glm::vec3 spherePos, float radius);

	private:
		static bool solveQuadratic(const float& a, const float& b, const float& c, float& x0, float& x1);
		
	};
}