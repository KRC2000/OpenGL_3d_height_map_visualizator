#pragma once

#include <glad/glad.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GlobalVars.h"

namespace fwork
{
	class Camera
	{
		bool mouselook = false;

		glm::mat4 view_mat = glm::mat4(1);
		glm::mat4 projection_mat = glm::mat4(1);


		float pitch = 0, yaw = -90.f;
		float lastX = SCR_WIDTH / 2, lastY = SCR_HEIGHT / 2;


		float forwardMovementSpeed = 2;
		float strafeMovementSpeed = 2;
		float backwardMovementSpeed = 2;
		float upDownMovementSpeed = 2;

		float mouseSens = 0.1f;

		bool firstMouseInput = true;
	public:
		glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	public:
		Camera()
		{

		};

		void applyCamera(unsigned int shaderProramID)
		{
			projection_mat = glm::perspective(glm::radians(70.0f), 800.f / 600.f, 0.001f, 100.0f);
			view_mat = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

			glUniformMatrix4fv(glGetUniformLocation(shaderProramID, "view_mat4"), 1, GL_FALSE, &view_mat[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(shaderProramID, "projection_mat4"), 1, GL_FALSE, &projection_mat[0][0]);
			glUniform3f(glGetUniformLocation(shaderProramID, "cameraPos"), cameraPos.x, cameraPos.y, cameraPos.z);
		};

		void mouseViewOrient(double xpos, double ypos)
		{
			if (mouselook)
			{
				if (firstMouseInput)
				{
					lastX = xpos;
					lastY = ypos;
					firstMouseInput = false;
				}

				float xoffset = xpos - lastX;
				float yoffset = lastY - ypos;
				lastX = xpos;
				lastY = ypos;

				xoffset *= mouseSens;
				yoffset *= mouseSens;

				yaw += xoffset;
				pitch += yoffset;

				if (pitch > 89.0f)
					pitch = 89.0f;
				if (pitch < -89.0f)
					pitch = -89.0f;

				glm::vec3 direction;
				direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
				direction.y = sin(glm::radians(pitch));
				direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
				cameraFront = glm::normalize(direction);
			}
		}

		void moveForward(float deltaTime)
		{
			cameraPos += forwardMovementSpeed * cameraFront * deltaTime;
		};

		void moveBackward(float deltaTime)
		{
			cameraPos -= forwardMovementSpeed * cameraFront * deltaTime;
		};

		void strafeLeft(float deltaTime)
		{
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * strafeMovementSpeed * deltaTime;
		};

		void strafeRight(float deltaTime)
		{
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * strafeMovementSpeed * deltaTime;
		};

		void moveUp(float deltaTime)
		{
			cameraPos += glm::vec3(0.f, upDownMovementSpeed * deltaTime, 0.f);
		};

		void moveDown(float deltaTime)
		{
			cameraPos -= glm::vec3(0.f, upDownMovementSpeed * deltaTime, 0.f);
		};

		void setMouseLook(bool mouselook) { this->mouselook = mouselook; if (mouselook == false) firstMouseInput = true; }
	};
}

