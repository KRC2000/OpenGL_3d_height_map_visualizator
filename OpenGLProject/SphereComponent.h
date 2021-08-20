#pragma once

#include <glad/glad.h>

namespace ecs
{
	class SphereComponent
	{
		unsigned int vao;
		unsigned int indicesAmount;
	public:

		void draw()
		{
			glBindVertexArray(vao);
			glDrawElements(GL_TRIANGLES, indicesAmount, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
	};
}