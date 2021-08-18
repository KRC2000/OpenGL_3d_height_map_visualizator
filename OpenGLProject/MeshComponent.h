#pragma once

#include <glad/glad.h>

#include "Component.h"
#include "Id.h"

#include "TransformComponent.h"

namespace ecs
{
	struct MeshComponent : public Component
	{
	private:	unsigned int texture_id = 0;
	public:
		unsigned int VAO_id;
		unsigned int indicesAmount;
		bool textured = false;

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
		}



	};

}