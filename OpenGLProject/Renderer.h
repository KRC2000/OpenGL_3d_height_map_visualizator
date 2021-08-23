#pragma once

#include <glad/glad.h>

#include "Shader.h"
#include "TransformComponent.h"
#include "MeshComponent.h"

namespace fwork
{
	class Renderer
	{
	public:
		void setWireframeMode(bool val)
		{
			(val) ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		// Draw mesh component with linked texture and transform
		void draw(ecs::MeshComponent& mesh_c, fwork::Shader& shader, glm::mat4 transform)
		{
			shader.setMat4("model_mat4", transform);
			shader.setVec3("meshColor", mesh_c.getColor());
			if (mesh_c.isTextured())
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, mesh_c.getTexture());
				shader.setBool("textured", true);
				shader.setInt("tex", 0);
			}
			else
			{
				shader.setBool("textured", false);
			}
			glBindVertexArray(mesh_c.getVAO());
			glDrawElements(GL_TRIANGLES, mesh_c.getIndicesAmount(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}

		// Draws mesh with texture at (0, 0, 0)(no transform applied)
		void draw(ecs::MeshComponent& mesh_c, fwork::Shader& shader)
		{
			shader.setVec3("meshColor", mesh_c.getColor());
			if (mesh_c.isTextured())
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, mesh_c.getTexture());
				shader.setBool("textured", true);
				shader.setInt("tex", 0);
			}
			else
			{
				shader.setBool("textured", false);
			}
			glBindVertexArray(mesh_c.getVAO());
			glDrawElements(GL_TRIANGLES, mesh_c.getIndicesAmount(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}

		// Draws without texture or any uniform setups(last binded texture may apply on mesh)
		void draw(ecs::MeshComponent& mesh_c)
		{
			glBindVertexArray(mesh_c.getVAO());
			glDrawElements(GL_TRIANGLES, mesh_c.getIndicesAmount(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
	};
}

