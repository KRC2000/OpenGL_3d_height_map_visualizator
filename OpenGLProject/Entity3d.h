#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace fwork
{
	class Entity3d
	{
		unsigned int VAO_id;
		unsigned int texture_id;
		unsigned int indicesAmount;
		bool textured = false;

		glm::mat4 model_mat4 = glm::mat4(1);
	public:
		Entity3d(unsigned int VAO_id, unsigned int indicesAmount):
			VAO_id(VAO_id), indicesAmount(indicesAmount)
		{}

		void setTexture(unsigned int textureId)
		{
			textured = true;
			texture_id = textureId;
		}

		void draw(Shader& shader)
		{
			if (textured)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texture_id);
			}
			
			shader.setMat4("model_mat4", model_mat4);

			glBindVertexArray(VAO_id);
			glDrawElements(GL_TRIANGLES, indicesAmount, GL_UNSIGNED_INT, 0);
		}

		void move(glm::vec3 vector)
		{
			model_mat4 = glm::translate(model_mat4, vector);
		}

		void rotate(float angle, glm::vec3 rotationAxis)
		{
			model_mat4 = glm::rotate(model_mat4, angle, rotationAxis);
		}

		void scale(glm::vec3 factor)
		{
			model_mat4 = glm::scale(model_mat4, factor);
		}

	};

}