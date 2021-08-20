#include "SizeComponent.h"

void ecs::SizeComponent::recalculateSize(const std::vector<float>& vertices, TransformComponent& trans_c, unsigned int offset)
{
	std::vector<float> transVertices;
	for (int i = 0; i < vertices.size(); i+=3 + offset)
	{
		//glm::vec4 vert = trans_c->getTransform() * glm::vec4(vertices[i], vertices[i+1], vertices[i+2], 1.f);
		glm::vec4 vert = /*trans_c->translation_mat **/ trans_c.rotation_mat * trans_c.scale_mat * glm::vec4(vertices[i], vertices[i+1], vertices[i+2], 1.f);

		transVertices.push_back(vert.x);
		transVertices.push_back(vert.y);
		transVertices.push_back(vert.z);
	}


	float maxX = transVertices[0], minX = transVertices[0],
		maxY = transVertices[1], minY = transVertices[1],
		maxZ = transVertices[2], minZ = transVertices[2];
	for (int i = 0; i < transVertices.size(); i += 3)
	{
		if (transVertices[i] > maxX) maxX = transVertices[i]; if (transVertices[i] < minX) minX = transVertices[i];
		if (transVertices[i + 1] > maxY) maxY = transVertices[i + 1]; if (transVertices[i + 1] < minY) minY = transVertices[i + 1];
		if (transVertices[i + 2] > maxZ) maxZ = transVertices[i + 2]; if (transVertices[i + 2] < minZ) minZ = transVertices[i + 2];
	}

	size.x= maxX - minX;
	size.y = maxY - minY;
	size.z = maxZ - minZ;
}
