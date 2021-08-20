#pragma once

#include <glad/glad.h>
#include <vector>

#include "Component.h"
#include "Id.h"

#include "TransformComponent.h"
#include "Shader.h"

namespace ecs
{
	class MeshComponent : public Component
	{
		unsigned int texture_id = 0;
		unsigned int VAO_id;
		unsigned int indicesAmount;
		bool isTextured = false;

		const std::vector<float>* verticesPtr;

	public:

		MeshComponent() { Component::id = ecs::ComponentId::MESH; }

		void setUp(unsigned int vao, unsigned int indicesAmount, const std::vector<float>& vertices ,bool isTextured)
		{
			this->VAO_id = vao;
			this->indicesAmount = indicesAmount;
			this->isTextured = isTextured;
			this->verticesPtr = &vertices;
		}

		void setTexture(unsigned int textureId) { texture_id = textureId; }

		const std::vector<float>& getVerticesRef()
		{
			return *verticesPtr;
		}

		void draw()
		{
			if (isTextured)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texture_id);
			}
			glBindVertexArray(VAO_id);
			glDrawElements(GL_TRIANGLES, indicesAmount, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}

		void draw(fwork::Shader& shader, ecs::TransformComponent& transform_c)
		{
			shader.setMat4("model_mat4", transform_c.getTransform());
			if (isTextured)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texture_id);
				shader.setBool("textured", true);
				shader.setInt("tex", 0);
			}
			else
			{
				shader.setBool("textured", false);
			}
			glBindVertexArray(VAO_id);
			glDrawElements(GL_TRIANGLES, indicesAmount, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
	};

}