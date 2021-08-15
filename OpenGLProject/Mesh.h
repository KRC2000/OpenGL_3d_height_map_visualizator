#pragma once
#include <vector>
#include <glad/glad.h>

#include "Shader.h"
#include "Camera.h"

namespace fwork
{	// Owns vertexes and indices data
	class Mesh
	{
		unsigned int id_VBO, id_EBO, id_VAO;
		unsigned int textureId;


		unsigned int vertices_arr_size = 0;
		unsigned int indices_arr_size = 0;

		std::vector<float>* vertices_arr = nullptr;
		std::vector<unsigned int>* indices_arr = nullptr;

		bool textured = false;
		bool elements = false;
		bool withTexCoords = false;

	public:
		Mesh() = delete;
		Mesh(std::vector<float>& vertices_arr, unsigned int vertices_arr_size, bool withTexCoords = false) :
			vertices_arr(&vertices_arr), vertices_arr_size(vertices_arr_size), withTexCoords(withTexCoords)
		{
			glGenVertexArrays(1, &id_VAO);
			glGenBuffers(1, &id_VBO);

			glBindVertexArray(id_VAO);


			glBindBuffer(GL_ARRAY_BUFFER, id_VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->vertices_arr_size, &this->vertices_arr->at(0), GL_STATIC_DRAW);

			if (!withTexCoords) glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			else glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			if (withTexCoords)
			{
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
				glEnableVertexAttribArray(1);
			}

			glBindVertexArray(0); // Unbinds first! Else other buffers will be unlinked to this VAO!
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		void setupIndices(std::vector<unsigned int>& indices_arr, unsigned int indices_arr_size)
		{
			this->indices_arr = &indices_arr;
			this->indices_arr_size = indices_arr_size;

			glGenBuffers(1, &id_EBO);

			glBindVertexArray(id_VAO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * this->indices_arr_size, &this->indices_arr->at(0), GL_STATIC_DRAW);

			glBindVertexArray(0); // Unbinds first! Else other buffers will be unlinked to this VAO!
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			elements = true;
		}

		void draw(Shader& shader, Camera& cam)
		{
			shader.use();
			cam.applyCamera(shader.ID);
			shader.setBool("textured", textured);

			if (textured)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, textureId);
				shader.setInt("t", 0);
			}


			glBindVertexArray(id_VAO);
			if (!elements) glDrawArrays(GL_TRIANGLES, 0, vertices_arr_size);
			else glDrawElements(GL_TRIANGLES, indices_arr_size, GL_UNSIGNED_INT, 0);
			//glDrawElementsInstanced(GL_TRIANGLES, indices_arr_size, GL_UNSIGNED_INT, 0, 100);
		}

		void setTexture(unsigned int id) { textured = true; textureId = id; }
	};
}
