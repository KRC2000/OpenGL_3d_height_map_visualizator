#pragma once

#include <glad/glad.h>

namespace fwork
{	// Owns vertexes and indices data
	class Mesh
	{
		unsigned int id_VBO, id_EBO, id_VAO;

		unsigned int vertices_arr_size = 0;
		unsigned int indices_arr_size = 0;

		float* vertices_arr = nullptr;
		unsigned int* indices_arr = nullptr;

		bool elements = false;

	public:
		Mesh() = delete;
		Mesh(float& vertices_arr, unsigned int vertices_arr_size) :
			vertices_arr(&vertices_arr), vertices_arr_size(vertices_arr_size)
		{
			glGenVertexArrays(1, &id_VAO);
			glGenBuffers(1, &id_VBO);

			glBindVertexArray(id_VAO);

			glBindBuffer(GL_ARRAY_BUFFER, id_VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->vertices_arr_size, this->vertices_arr, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			glBindVertexArray(0); // Unbinds first! Else other buffers will be unlinked to this VAO!
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		void setupIndices(unsigned int& indices_arr, unsigned int indices_arr_size)
		{
			this->indices_arr = &indices_arr;
			this->indices_arr_size = indices_arr_size;

			glGenBuffers(1, &id_EBO);

			glBindVertexArray(id_VAO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * this->indices_arr_size, this->indices_arr, GL_STATIC_DRAW);

			glBindVertexArray(0); // Unbinds first! Else other buffers will be unlinked to this VAO!
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			elements = true;
		}

		void draw()
		{
			glBindVertexArray(id_VAO);
			if (!elements) glDrawArrays(GL_TRIANGLES, 0, vertices_arr_size);
			else glDrawElements(GL_TRIANGLES, indices_arr_size, GL_UNSIGNED_INT, 0);
		}
	};
}
