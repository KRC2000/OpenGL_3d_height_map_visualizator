#include "MeshManager.h"

void fwork::MeshManager::calculateMeshSize(Vec3& size, const std::vector<float>& vertices, unsigned int offset)
{
	float maxX = vertices[0], minX = vertices[0],
		maxY = vertices[1], minY = vertices[1],
		maxZ = vertices[2], minZ = vertices[2];
	for (int i = 0; i < vertices.size(); i += 3 + offset)
	{
		if (vertices[i] > maxX) maxX = vertices[i]; if (vertices[i] < minX) minX = vertices[i];
		if (vertices[i + 1] > maxY) maxY = vertices[i + 1]; if (vertices[i + 1] < minY) minY = vertices[i + 1];
		if (vertices[i + 2] > maxZ) maxZ = vertices[i + 2]; if (vertices[i + 2] < minZ) minZ = vertices[i + 2];
	}

	size.x = maxX - minX;
	size.y = maxY - minY;
	size.z = maxZ - minZ;
}
