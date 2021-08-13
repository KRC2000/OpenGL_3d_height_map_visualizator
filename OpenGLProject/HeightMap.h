#pragma once

#include <iostream>

#include <glad/glad.h>
#include <glfw3.h>

#include <glm/glm.hpp>

#include "imgui.h"
#include "stb_image.h"
#include "Shader.h"
#include "Camera.h"

namespace fwork
{
	class HeightMap
	{
		unsigned int heightmap_texture_with;
		unsigned int heightmap_texture_height;

		unsigned int id_texture_heightmap;

		int vertices_arr_size = 0;
		int indices_arr_size = 0;

		fwork::Shader* shader = nullptr;
		fwork::Shader* shader_normalsdraw = nullptr;
		float* vertices_arr = nullptr;
		unsigned int* indices_arr = nullptr;

		unsigned int id_mapVBO, id_mapEBO, id_mapVAO;

	public:
		glm::vec3 targetColor{ 1, 1, 1 };
		float heightMultiplier = 1;
		float normalLength = 1;
		bool wireframe = false;
		bool textured = true;
		bool normals = true;
		bool light = true;

		HeightMap(std::string texture_path, const char* shadersFolderPath)
		{
			std::string path_shader_mesh_vert(shadersFolderPath); path_shader_mesh_vert += "/3.3.heightmap.vert";
			std::string path_shader_mesh_geom(shadersFolderPath); path_shader_mesh_geom += "/3.3.heightmap.geom";
			std::string path_shader_mesh_frag(shadersFolderPath); path_shader_mesh_frag += "/3.3.heightmap.frag";
			std::string path_shader_normals_vert(shadersFolderPath); path_shader_normals_vert += "/3.3.heightmap_normal.vert";
			std::string path_shader_normals_geom(shadersFolderPath); path_shader_normals_geom += "/3.3.heightmap_normal.geom";
			std::string path_shader_normals_frag(shadersFolderPath); path_shader_normals_frag += "/3.3.heightmap_normal.frag";

			shader = new fwork::Shader(path_shader_mesh_vert.c_str(), path_shader_mesh_frag.c_str(), path_shader_mesh_geom.c_str());
			shader_normalsdraw = new fwork::Shader(path_shader_normals_vert.c_str(), path_shader_normals_frag.c_str(), path_shader_normals_geom.c_str());
			std::cout << path_shader_mesh_vert;
			loadHeightmapTexture(texture_path);

			vertices_arr = genHeightMapPlane(indices_arr, heightmap_texture_with, heightmap_texture_height, 100, vertices_arr_size, indices_arr_size);


			glGenVertexArrays(1, &id_mapVAO);
			glGenBuffers(1, &id_mapVBO);
			glGenBuffers(1, &id_mapEBO);

			glBindVertexArray(id_mapVAO);

			glBindBuffer(GL_ARRAY_BUFFER, id_mapVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices_arr_size, vertices_arr, GL_STATIC_DRAW);
			std::cout << sizeof(*vertices_arr) * vertices_arr_size << std::endl;

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_mapEBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices_arr_size, indices_arr, GL_STATIC_DRAW);
			std::cout << sizeof(*indices_arr) * indices_arr_size << std::endl;

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			glBindVertexArray(0); // Unbinds first! Else other buffers will be unlinked to this VAO!
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		};

		void draw(fwork::Camera& cam)
		{
			if (!wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			else glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, id_texture_heightmap);

			shader->use();
			cam.applyCamera(shader->ID);
			shader->setInt("hMap", 0);
			shader->setFloat("heightMultiplier", heightMultiplier);
			shader->setBool("textured", textured);
			shader->setBool("light", light);
			glBindVertexArray(id_mapVAO);
			glDrawElements(GL_TRIANGLES, indices_arr_size, GL_UNSIGNED_INT, 0);


			if (normals)
			{
				shader_normalsdraw->use();
				cam.applyCamera(shader_normalsdraw->ID);
				shader_normalsdraw->setInt("hMap", 0);
				shader_normalsdraw->setFloat("heightMultiplier", heightMultiplier);
				shader_normalsdraw->setFloat("normalLength", normalLength);
				shader_normalsdraw->setBool("textured", textured);
				glBindVertexArray(id_mapVAO);
				//glDrawArrays(GL_TRIANGLES, 0, vertices_arr_size*2);
				glDrawElements(GL_TRIANGLES, indices_arr_size, GL_UNSIGNED_INT, 0);
				//glDrawElements(GL_LINES, indices_arr_size, GL_UNSIGNED_INT, 0);

			}

			drawUI(cam);
		}

		void terminate()
		{
			glDeleteVertexArrays(1, &id_mapVAO);
			glDeleteBuffers(1, &id_mapVBO);
			glDeleteBuffers(1, &id_mapEBO);
		}

		fwork::Shader* getShader() { return shader; }

	private:
		void drawUI(fwork::Camera& cam)
		{
			// render your GUI
			ImGui::Begin("Settings");
			ImGui::Text("'Left-Alt' to switch cursor mode");
			ImGui::BulletText(("X: " + std::to_string(cam.cameraPos.x)).c_str());
			ImGui::BulletText(("Y: " + std::to_string(cam.cameraPos.y)).c_str());
			ImGui::BulletText(("Z: " + std::to_string(cam.cameraPos.z)).c_str());
			ImGui::SliderFloat("Height multiplier", &heightMultiplier, 0.00001f, 20.0f, "%.6f");
			ImGui::Checkbox("Wireframe mode", &wireframe);
			ImGui::Checkbox("Textured", &textured);
			ImGui::Checkbox("Normals", &normals);
			if (normals) ImGui::SliderFloat("Normal length", &normalLength, 0.01, 3);
			ImGui::Checkbox("Light", &light);

			ImGui::Text("'Left-Alt' for free camera");
			ImGui::ColorEdit3("color 1", &targetColor[0]);
			//ImGui::ShowDemoWindow();
			ImGui::End();
		}

		// Yet works just with TGA. Returns image data only Green. Fills "width" and "height" with image size
		void loadHeightmapTexture(std::string path)
		{
			glGenTextures(1, &id_texture_heightmap);
			glBindTexture(GL_TEXTURE_2D, id_texture_heightmap);
			// set the texture wrapping parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			// set texture filtering parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			// load image, create texture and generate mipmaps
			int width, height, nrChannels;
			//stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
			unsigned char* data = stbi_load("map.tga", &width, &height, &nrChannels, 0);
			if (data)
			{
				if (width < INT_MAX && height < INT_MAX)
				{
					heightmap_texture_with = width;
					heightmap_texture_with = height;
				}

				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			else
			{
				std::cout << "Failed to load texture" << std::endl;
			}
			stbi_image_free(data);
		}

		// Returns pointer to array with vertices. 
		// Fills "drawIndices"(MUST BE = nullptr!) with draw indices array, "returnArr_size" with size of returned array
		// "freq" - (freq+1) * (freq+1) points will be created on the plane

		float* genHeightMapPlane(unsigned int*& drawIndices, unsigned int map_width, unsigned int map_height, int freq, int& returnArr_size, int& indicesAmount)
		{
			int vertWith = (freq != 0) ? freq + 1 : map_width;
			int vertHeight = (freq != 0) ? freq + 1 : map_height;
			int vertAmount = vertWith * vertHeight;


			// Creating array of needed size
			returnArr_size = vertAmount * 3;
			float* vertices = vertices = new float[vertAmount * 3];

			float widthStep = 2.f / (vertWith - 1);
			float heightStep = 2.f / (vertHeight - 1);

			// Filling array with vertices positions, not ready to draw because points aren't in triangle sequence
			int vert_i = 0;
			for (int z = 0; z < vertHeight; z++)
			{
				for (int x = 0; x < vertWith; x++)
				{
					vertices[vert_i] = -1.f + x * widthStep;
					vertices[vert_i + 1] = 0.f;
					vertices[vert_i + 2] = 1.f + z * -heightStep;

					//std::cout << vertices[vert_i] << " " << vertices[vert_i +1] << " " << vertices[vert_i +2] << std::endl;

					vert_i += 3;
				}
			}
			//std::cout << std::endl;
			// Indices for upper-left triangles
			//  *---*
			//  |  /
			//  | /
			//  |/
			//  *
			indicesAmount = freq * freq * 6;
			drawIndices = new unsigned int[indicesAmount];
			int ind_i = 0;
			for (int y = 0; y < vertHeight - 1; y++)
			{
				for (int x = 0; x < vertWith - 1; x++)
				{
					drawIndices[ind_i] = y * vertWith + x;
					drawIndices[ind_i + 1] = y * vertWith + x + 1;
					drawIndices[ind_i + 2] = y * vertWith + x + vertWith;

					//std::cout << drawIndices[ind_i] << " " << drawIndices[ind_i+1] << " " << drawIndices[ind_i+2] << std::endl;
					ind_i += 3;
				}
			}

			//std::cout << std::endl;

			// Indices for down-right triangles
			//      *
			//     /|
			//    / |
			//   /  |
			//  *---*
			for (int y = 0; y < vertHeight - 1; y++)
			{
				for (int x = 1; x < vertWith; x++)
				{
					drawIndices[ind_i] = y * vertWith + x;
					drawIndices[ind_i + 1] = y * vertWith + x + vertWith;
					drawIndices[ind_i + 2] = y * vertWith + x + vertWith - 1;
					//std::cout << drawIndices[ind_i] << " " << drawIndices[ind_i + 1] << " " << drawIndices[ind_i + 2] << std::endl;
					ind_i += 3;
				}
			}

			return vertices;
		}

	};

	// How many triangles?
	// Var "freq" will describe how many triangles will be generated
	// freq = 0 - vertex per height map pixel
	// freq = 1 - two triangles that form rectangle
	// freq = 2 - two triangles that form rectangle * 4
	// freq = 3 - two triangles that form rectangle * 9
	// 
	// 	   *---*
	// 	   |  /|
	// 	   | / |   <- freq = 1
	// 	   |/  |
	// 	   *---*
	// 
	// 	   *---*---*
	// 	   |  /|  /|
	// 	   | / | / |
	// 	   |/  |/  |   <- freq = 2
	// 	   *---*---*
	// 	   |  /|  /|
	// 	   | / | / |
	// 	   |/  |/  |
	// 	   *---*---*
	// 
	// 	   *---*---*---*
	// 	   |  /|  /|  /|
	// 	   | / | / | / |
	// 	   |/  |/  |/  |   <- freq = 3
	// 	   *---*---*---*
	// 	   |  /|  /|  /|
	// 	   | / | / | / |
	// 	   |/  |/  |/  |
	// 	   *---*---*---*
	// 	   |  /|  /|  /|
	// 	   | / | / | / |
	// 	   |/  |/  |/  |
	// 	   *---*---*---*
}

