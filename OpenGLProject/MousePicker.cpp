#include "MousePicker.h"

glm::vec3 fwork::MousePicker::getPlaneIntersection(Camera& cam, GLFWwindow& window, glm::vec3 planePoint, glm::vec3 planeNorm)
{
	if (glfwGetInputMode(&window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL)
	{
		glm::vec3 ray = getMouseRay(cam, window);
		float distance = glm::dot((glm::vec3(0, 0, 0) - cam.cameraPos), glm::vec3(0, 1, 0)) / glm::dot(ray, glm::vec3(0, 1, 0));
		return (ray * distance + cam.cameraPos);
	}
	else
	{
		glm::vec3 ray = cam.cameraFront;
		float distance = glm::dot((glm::vec3(0, 0, 0) - cam.cameraPos), glm::vec3(0, 1, 0)) / glm::dot(ray, glm::vec3(0, 1, 0));
		return (ray * distance + cam.cameraPos);
	}

}