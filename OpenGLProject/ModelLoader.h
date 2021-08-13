#pragma once
#include <iostream>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace fwork
{
	// Loads vertices and indices of model in vectors. Stores them as long as loadModel() is not being called again.
	class ModelLoader
	{
		std::vector<float> vertices;
		std::vector<unsigned int> indices;

	public:

		ModelLoader() {}

		void loadModel(std::string path)
		{
			vertices.clear();
			indices.clear();

			Assimp::Importer importer;

			const aiScene* scene = importer.ReadFile("model.obj", aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);
			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			{
				std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
			}


			for (int i = 0; i < scene->mMeshes[0]->mNumVertices; ++i)
			{
				//std::cout << scene->mMeshes[0]->mVertices[i].x << ";  " << scene->mMeshes[0]->mVertices[i].y << ";  " << scene->mMeshes[0]->mVertices[i].z << std::endl;

				vertices.push_back(scene->mMeshes[0]->mVertices[i].x);
				vertices.push_back(scene->mMeshes[0]->mVertices[i].y);
				vertices.push_back(scene->mMeshes[0]->mVertices[i].z);
			}
			std::cout << "Loaded " << vertices.size() / 3 << " vertices" << std::endl;


			for (unsigned int i = 0; i < scene->mMeshes[0]->mNumFaces; ++i)
			{
				aiFace face = scene->mMeshes[0]->mFaces[i];
				for (unsigned int j = 0; j < face.mNumIndices; ++j)
				{
					//std::cout << face.mIndices[j] << std::endl;
					indices.push_back(face.mIndices[j]);
				}
			}
			std::cout << "Loaded " << indices.size() << " indices" << std::endl;
		}

		std::vector<float> getVertices()
		{
			return vertices;
		}

		std::vector<unsigned int> getIndices()
		{
			return indices;
		}
	};
}
