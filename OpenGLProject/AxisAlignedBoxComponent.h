#pragma once

#include <glad/glad.h>

#include "Component.h"
#include "Id.h"

#include "TransformComponent.h"
#include "AlignedBox.h"

namespace ecs
{
	class AxisAlignedBoxComponent: public Component
	{
		unsigned int VAO, EBO, VBO;
	public:
		fwork::AlignedBox box;
		AxisAlignedBoxComponent() { Component::id = ComponentId::AXISALIGNEDBOX; }
		void init(const std::vector<float>& vertices, int offset = 0)
		{
			box.initFromVertices(vertices, offset);
			
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);
			glGenVertexArrays(1, &VAO);

			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * box.getVertices().size(), &box.getVertices()[0], GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * fwork::AlignedBoxIndices.size(), &fwork::AlignedBoxIndices[0], GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		void draw()
		{
			glBindVertexArray(VAO);
			glDrawElements(GL_LINES, fwork::AlignedBoxIndices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
	};
}