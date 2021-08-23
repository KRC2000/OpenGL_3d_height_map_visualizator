#include "MousePicker.h"

fwork::Camera* fwork::MousePicker::camPtr = nullptr;
GLFWwindow* fwork::MousePicker::windowPtr = nullptr;

void fwork::MousePicker::bindWindowCamera(GLFWwindow& window, Camera& cam)
{
	windowPtr = &window;
	camPtr = &cam;
}

glm::vec3 fwork::MousePicker::getPointAlongRay(Camera& cam, GLFWwindow& window, float distance)
{
	return (getMouseRay(cam, window) * distance + cam.cameraPos);
}

glm::vec3 fwork::MousePicker::getMouseRay(Camera& cam, GLFWwindow& window)
{
	glm::vec3 ray_world;
	if (glfwGetInputMode(&window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL)
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
		ray_world = glm::normalize(glm::vec3(ray.x, ray.y, ray.z));
	}
	else
		ray_world = cam.cameraFront;



	return ray_world;
}

glm::vec3 fwork::MousePicker::getPlaneIntersection(Camera& cam, GLFWwindow& window, glm::vec3 planePoint, glm::vec3 planeNorm)
{
	glm::vec3 ray = getMouseRay(cam, window);
	float distance = glm::dot((glm::vec3(0, 0, 0) - cam.cameraPos), glm::vec3(0, 1, 0)) / glm::dot(ray, glm::vec3(0, 1, 0));
	return (ray * distance + cam.cameraPos);
}

bool fwork::MousePicker::getSphereIntersection(Camera& cam, GLFWwindow& window, glm::vec3 spherePos, float radius, glm::vec3& intersectionPoint)
{
	glm::vec3 ray = getMouseRay(cam, window);
	float t0, t1; // solutions for t if the ray intersects 

#if 0
		// geometric solution
	glm::vec3 L = spherePos - cam.cameraPos;
	float tca = glm::dot(L, ray);
	// if (tca < 0) return false;
	float d2 = glm::dot(L, L) - tca * tca;
	if (d2 > radius * radius) return false;
	float thc = sqrt(radius * radius - d2);
	t0 = tca - thc;
	t1 = tca + thc;

#else
		// analytic solution
	glm::vec3 L = cam.cameraPos - spherePos;
	float a = glm::dot(ray, ray);
	float b = 2 * glm::dot(ray, L);
	float c = glm::dot(L, L) - radius * radius;
	if (!solveQuadratic(a, b, c, t0, t1)) return false;
#endif 
	if (t0 > t1) std::swap(t0, t1);

	if (t0 < 0) {
		t0 = t1; // if t0 is negative, let's use t1 instead 
		if (t0 < 0) return false; // both t0 and t1 are negative 
	}

	intersectionPoint = (ray * t0 + cam.cameraPos);

	return true;
}

bool fwork::MousePicker::getSphereIntersection(glm::vec3 spherePos, float radius, glm::vec3& intersectionPoint)
{
	glm::vec3 ray = getMouseRay(*camPtr, *windowPtr);
	float t0, t1; // solutions for t if the ray intersects 

#if 0
		// geometric solution
	glm::vec3 L = spherePos - camPtr->cameraPos;
	float tca = glm::dot(L, ray);
	// if (tca < 0) return false;
	float d2 = glm::dot(L, L) - tca * tca;
	if (d2 > radius * radius) return false;
	float thc = sqrt(radius * radius - d2);
	t0 = tca - thc;
	t1 = tca + thc;

#else
		// analytic solution
	glm::vec3 L = camPtr->cameraPos - spherePos;
	float a = glm::dot(ray, ray);
	float b = 2 * glm::dot(ray, L);
	float c = glm::dot(L, L) - radius * radius;
	if (!solveQuadratic(a, b, c, t0, t1)) return false;
#endif 
	if (t0 > t1) std::swap(t0, t1);

	if (t0 < 0) {
		t0 = t1; // if t0 is negative, let's use t1 instead 
		if (t0 < 0) return false; // both t0 and t1 are negative 
	}

	intersectionPoint = (ray * t0 + camPtr->cameraPos);

	return true;
}

bool fwork::MousePicker::getSphereIntersection(glm::vec3 spherePos, float radius)
{
	glm::vec3 ray = getMouseRay(*camPtr, *windowPtr);
	float t0, t1; // solutions for t if the ray intersects 

#if 0
		// geometric solution
	glm::vec3 L = spherePos - camPtr->cameraPos;
	float tca = glm::dot(L, ray);
	// if (tca < 0) return false;
	float d2 = glm::dot(L, L) - tca * tca;
	if (d2 > radius * radius) return false;
	float thc = sqrt(radius * radius - d2);
	t0 = tca - thc;
	t1 = tca + thc;

#else
		// analytic solution
	glm::vec3 L = camPtr->cameraPos - spherePos;
	float a = glm::dot(ray, ray);
	float b = 2 * glm::dot(ray, L);
	float c = glm::dot(L, L) - radius * radius;
	if (!solveQuadratic(a, b, c, t0, t1)) return false;
#endif 
	if (t0 > t1) std::swap(t0, t1);

	if (t0 < 0) {
		t0 = t1; // if t0 is negative, let's use t1 instead 
		if (t0 < 0) return false; // both t0 and t1 are negative 
	}

	return true;
}

bool fwork::MousePicker::solveQuadratic(const float& a, const float& b, const float& c, float& x0, float& x1)
{
	float discr = b * b - 4 * a * c;
	if (discr < 0) return false;
	else if (discr == 0) x0 = x1 = -0.5 * b / a;
	else {
		float q = (b > 0) ?
			-0.5 * (b + sqrt(discr)) :
			-0.5 * (b - sqrt(discr));
		x0 = q / a;
		x1 = c / q;
	}
	if (x0 > x1) std::swap(x0, x1);

	return true;
}