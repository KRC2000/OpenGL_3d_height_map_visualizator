#pragma once

#include <glad/glad.h>

#include "Component.h"
#include "Id.h"

#include "TransformComponent.h"
#include "AlignedBox.h"

namespace ecs
{
	struct MeshComponent : public Component
	{
	private:	
		unsigned int texture_id = 0;
		unsigned int boxVBO, boxVAO;
	public:
		unsigned int VAO_id;
		unsigned int indicesAmount;
		bool textured = false;
		fwork::AlignedBox box;

		MeshComponent()
		{
			Component::id = ecs::ComponentId::MESH;
		}

		void setTexture(unsigned int textureId)
		{
			texture_id = textureId;
		}

		void draw()
		{
			if (textured)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texture_id);
			}
			glBindVertexArray(VAO_id);
			glDrawElements(GL_TRIANGLES, indicesAmount, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

		}

		void setupDebagDraw()
		{
			glGenVertexArrays(1, &boxVAO);
			glBindVertexArray(boxVAO);

			glGenBuffers(1, &boxVBO);
			glBindBuffer(GL_ARRAY_BUFFER, boxVBO);
			glBufferData(GL_ARRAY_BUFFER, box.getVertices().size() * sizeof(float), &box.getVertices()[0], GL_STATIC_DRAW); 
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		void debugDraw()
		{
			glBindVertexArray(boxVAO);
			glDrawArrays(GL_LINE_STRIP, 0, box.getVertices().size()/3);
		}

	};

}