#pragma once

#include <vector>

#include "Component.h"
#include "Id.h"


namespace ecs
{
	// Stores info needed for drawing complete 3d object with texture: 
	// VAO, texture id, indices amount, pointer to vertices(can be useful for other components)
	class MeshComponent : public Component
	{
		unsigned int texture_id = 0;
		unsigned int VAO_id;
		unsigned int indicesAmount;
		bool istextured = false;

		const std::vector<float>* verticesPtr;

	public:

		MeshComponent() { Component::id = ecs::ComponentId::MESH; }

		void setTexture(unsigned int textureId) { texture_id = textureId; }
		bool isTextured() { return istextured; }
		unsigned int getTexture() { return texture_id; }
		unsigned int getVAO() { return VAO_id; }
		unsigned int getIndicesAmount() { return indicesAmount; }
		const std::vector<float>& getVerticesRef() { return *verticesPtr; }

		void setUp(unsigned int vao, unsigned int indicesAmount, const std::vector<float>& vertices ,bool isTextured)
		{
			this->VAO_id = vao;
			this->indicesAmount = indicesAmount;
			this->istextured = isTextured;
			this->verticesPtr = &vertices;
		}
	};

}