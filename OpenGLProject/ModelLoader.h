#pragma once
#include <iostream>
#include <vector>

#include <glad/glad.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


namespace fwork
{
	// Loads vertices and indices of model in vectors. Stores them as long as loadModel() is not being called again.
	class ModelLoader
	{
		std::string textureName = "";

	public:

		ModelLoader() {}

		// param "texCoordsToVert" - must texture coordinates be inserted in "vert" vector
		void loadModel(std::string path, std::vector<float>& vert, std::vector<unsigned int>& ind)
		{
			vert.clear();
			ind.clear();
			textureName = "";

			Assimp::Importer importer;

			const aiScene* scene = importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs |aiProcess_JoinIdenticalVertices);
			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			{
				std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
			}

			

			for (int i = 0; i < scene->mMeshes[0]->mNumVertices; ++i)
			{
				//std::cout << scene->mMeshes[0]->mVertices[i].x << ";  " << scene->mMeshes[0]->mVertices[i].y << ";  " << scene->mMeshes[0]->mVertices[i].z << std::endl;

				vert.push_back(scene->mMeshes[0]->mVertices[i].x);
				vert.push_back(scene->mMeshes[0]->mVertices[i].y);
				vert.push_back(scene->mMeshes[0]->mVertices[i].z);

				if (scene->mMeshes[0]->mTextureCoords[0])
				{
					vert.push_back(scene->mMeshes[0]->mTextureCoords[0][i].x);
					vert.push_back(scene->mMeshes[0]->mTextureCoords[0][i].y);
					//std::cout << scene->mMeshes[0]->mTextureCoords[0][i].x << ";  " << scene->mMeshes[0]->mTextureCoords[0][i].y << std::endl;
				}
			}

			std::cout << "Loaded " << vert.size() / 3 << " vertices" << std::endl;


			for (unsigned int i = 0; i < scene->mMeshes[0]->mNumFaces; ++i)
			{
				aiFace face = scene->mMeshes[0]->mFaces[i];
				for (unsigned int j = 0; j < face.mNumIndices; ++j)
				{
					//std::cout << face.mIndices[j] << std::endl;
					ind.push_back(face.mIndices[j]);
				}
			}
			std::cout << "Loaded " << ind.size() << " indices" << std::endl;


			unsigned int mat_i = scene->mMeshes[0]->mMaterialIndex;
			aiString t_path;
			scene->mMaterials[mat_i]->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &t_path);
			textureName = t_path.C_Str();

		}

		std::string getTextureFileName() { return textureName; }

	};
}
