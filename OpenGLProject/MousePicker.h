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
		MousePicker()
		{

		}

		void update(GLFWwindow& window, Camera& cam)
		{
			glm::dvec2 mousePos;
			glfwGetCursorPos(&window, &mousePos.x, &mousePos.y);

			glm::ivec2 windowSize;
			glfwGetWindowSize(&window, &windowSize.x, &windowSize.y);

			glm::vec2 mousePos_normalised;
			mousePos_normalised.x = 2 * mousePos.x / windowSize.x - 1;
			mousePos_normalised.y = 2 * mousePos.y / windowSize.y - 1;

			glm::vec4 ray(mousePos_normalised.x, mousePos_normalised.y, -1.f, 1.f);

			ray = glm::inverse(cam.getProjectionMat()) * ray;
			ray = glm::vec4(ray.x, ray.y, -1.0, 0.0);
			ray = glm::inverse(cam.getViewMat()) * ray;
			glm::vec3 ray_world = glm::normalize(glm::vec3(ray.x, ray.y, ray.z));

			glm::vec3 point = cam.cameraPos + ray_world * 3.f;
			//std::cout << point.x << "  " << point.y << "  " << point.z << std::endl;
		}

		glm::vec3 getMouseRay(Camera& cam, GLFWwindow& window)
		{
			glm::dvec2 mousePos;
			glfwGetCursorPos(&window, &mousePos.x, &mousePos.y);
			glm::ivec2 windowSize;
			glfwGetWindowSize(&window, &windowSize.x, &windowSize.y);

			glm::vec2 mousePos_normalised;
			mousePos_normalised.x = (2 * mousePos.x) / windowSize.x - 1.f;
			mousePos_normalised.y = 1.f - (2 * mousePos.y) / windowSize.y;

			glm::vec4 ray(mousePos_normalised.x, mousePos_normalised.y, -1.f, 1.f);

			ray = glm::inverse(cam.getProjectionMat()) * ray;
			ray = glm::vec4(ray.x, ray.y, -1.0f, 0.0);
			ray = glm::inverse(cam.getViewMat()) * ray;
			glm::vec3 ray_world = glm::normalize(glm::vec3(ray.x, ray.y, ray.z));

			return ray_world;
		}

		glm::vec3 getPlaneIntersection(Camera& cam, GLFWwindow& window, glm::vec3 planePoint, glm::vec3 planeNorm);
		

	};
}