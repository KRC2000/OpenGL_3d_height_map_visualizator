#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <glad/glad.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "MeshComponent.h"
#include "AlignedBox.h"

namespace fwork
{
	class MeshManager
	{
		// name == path
		struct Mesh
		{
			unsigned int VBO_id, EBO_id, VAO_id;
			std::vector<float> vertices;
			std::vector<unsigned int> indices;
			std::string name;
			std::string texPath;
			bool texCoords;
			//fwork::AlignedBox box;
		};

		/*std::vector<float> vertices;
		std::vector<unsigned int> indices;*/

		std::vector<Mesh> meshes;

	public:
		MeshManager()
		{}

		// Only loads first mesh in loaded scene
		void loadMesh(std::string path)
		{
			Mesh mesh;

			/*vertices.clear();
			indices.clear();*/

			Assimp::Importer importer;
			const aiScene* scene = getSceneObjectRef(path, importer);
		
		
			loadVertsToVec(scene, mesh.vertices, mesh.texCoords);
			loadIndicesToVec(scene, mesh.indices);

			createAndLoadBuffers(mesh, mesh.vertices, mesh.indices);

			mesh.name = path;
			mesh.texPath = getTexturePath(scene);
			//AlignedBox box2(vertices, 2);
			//mesh.box = box2;
			
			meshes.push_back(mesh);

			std::cout << "MeshManager::Mesh successfully loaded\n";
		}

		void setUpMeshComponent(ecs::MeshComponent& mesh_c, std::string meshName)
		{
			for (Mesh& mesh : meshes)
			{
				if (mesh.name == meshName)
				{
					mesh_c.VAO_id = mesh.VAO_id;
					mesh_c.indicesAmount = mesh.indices.size();
					mesh_c.textured = mesh.texCoords;
					//mesh_c.box = mesh.box;
				}
			}
		}

		unsigned int getVAO(std::string meshName)
		{
			for (Mesh& mesh : meshes)
			{
				if (mesh.name == meshName) return mesh.VAO_id;
			}
			std::cout << "MeshManager::Can't get VAO, no mesh with such name\n";
			return 0;
		}

		// Returns local path relative to mesh object(!)
		std::string getMeshTexturePath(std::string meshName)
		{
			for (Mesh& mesh : meshes)
			{
				if (mesh.name == meshName) return mesh.texPath;
			}
			std::cout << "MeshManager::Can't get mesh texture path, no mesh with such name\n";
			return "";
		}

		// Returns vertices vector copy 
		std::vector<float> getMeshVertices(std::string meshName)
		{
			for (Mesh& mesh : meshes)
			{
				if (mesh.name == meshName) return mesh.vertices;
			}
		}

		/*unsigned int getMeshIndicesAmount(std::string meshName)
		{
			for (Mesh& mesh : meshes)
			{
				if (mesh.name == meshName) return mesh.indicesAmount;
			}
			std::cout << "MeshManager::Can't get indices amount, no mesh with such name\n";
			return 0;
		}*/

		// Returns vector copy!
		//std::vector<float> getLastLoadedVertices() { return vertices; }

	private:

		// Loads mesh file in aiScene - assimp structure
		const aiScene* getSceneObjectRef(std::string path, Assimp::Importer& importer)
		{
			const aiScene* scene = importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);
			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			{
				std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
			}

			return scene;
		}

		// Pushes back vertice and texture(if present) coordinates in vector
		// param "texCoords" filled true if texture coords are present, false otherwise
		// Only first found mesh!
		void loadVertsToVec(const aiScene* scene, std::vector<float>& vertices, bool& texCoords)
		{

			if (scene->mMeshes[0]->mTextureCoords[0] == NULL) texCoords = false;
			else texCoords = true;

			for (int i = 0; i < scene->mMeshes[0]->mNumVertices; ++i)
			{
				//std::cout << scene->mMeshes[0]->mVertices[i].x << ";  " << scene->mMeshes[0]->mVertices[i].y << ";  " << scene->mMeshes[0]->mVertices[i].z << std::endl;

				vertices.push_back(scene->mMeshes[0]->mVertices[i].x);
				vertices.push_back(scene->mMeshes[0]->mVertices[i].y);
				vertices.push_back(scene->mMeshes[0]->mVertices[i].z);

				if (texCoords)
				{
					vertices.push_back(scene->mMeshes[0]->mTextureCoords[0][i].x);
					vertices.push_back(scene->mMeshes[0]->mTextureCoords[0][i].y);
					//std::cout << scene->mMeshes[0]->mTextureCoords[0][i].x << ";  " << scene->mMeshes[0]->mTextureCoords[0][i].y << std::endl;
				}
			}
		}

		// Fills vector with indices
		// OF only first found mesh!
		void loadIndicesToVec(const aiScene* scene, std::vector<unsigned int>& indices)
		{
			for (unsigned int i = 0; i < scene->mMeshes[0]->mNumFaces; ++i)
			{
				aiFace face = scene->mMeshes[0]->mFaces[i];
				for (unsigned int j = 0; j < face.mNumIndices; ++j)
				{
					//std::cout << face.mIndices[j] << std::endl;
					indices.push_back(face.mIndices[j]);
				}
			}
		}

		// Returns mesh's texture file path
		// Texture path of first found material of first found mesh!
		std::string getTexturePath(const aiScene* scene)
		{
			aiString t_path;
			scene->mMaterials[scene->mMeshes[0]->mMaterialIndex]->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &t_path);
			std::string path = t_path.C_Str();
			return path;
		}

		// Generate buffers, buffers vertex and index data, sets vertex array buffer
		// If no texture coordinates in "vertices" - VertexAttribArray number 1 is not enabled
		void createAndLoadBuffers(Mesh& mesh, std::vector<float>& vertices, std::vector<unsigned int>& indices)
		{
			glGenVertexArrays(1, &mesh.VAO_id);
			glGenBuffers(1, &mesh.VBO_id);

			glBindVertexArray(mesh.VAO_id);


			glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO_id);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

			if (!mesh.texCoords) glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			else glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			if (mesh.texCoords)
			{
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
				glEnableVertexAttribArray(1);
			}

			glGenBuffers(1, &mesh.EBO_id);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO_id);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);

			glBindVertexArray(0); // Unbinds first! Else other buffers will be unlinked to this VAO!
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
	};
}