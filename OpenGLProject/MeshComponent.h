#pragma once

#include <glad/glad.h>

#include "Component.h"
#include "Id.h"

#include "TransformComponent.h"
#include "AlignedBox.h"

namespace ecs
{
	class MeshComponent : public Component
	{
		unsigned int texture_id = 0;
		unsigned int VAO_id;
		unsigned int indicesAmount;
		bool isTextured = false;
	public:

		MeshComponent() { Component::id = ecs::ComponentId::MESH; }

		void setUp(unsigned int vao, unsigned int indicesAmount, bool isTextured)
		{
			this->VAO_id = vao;
			this->indicesAmount = indicesAmount;
			this->isTextured = isTextured;
		}

		void setTexture(unsigned int textureId) { texture_id = textureId; }

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

	};

}