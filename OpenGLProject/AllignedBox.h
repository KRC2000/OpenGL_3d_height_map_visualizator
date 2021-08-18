#pragma once

#include <vector>

#include "Vec3.h"

namespace fwork
{
	class AllignedBox
	{
		Vec3 centerPos;
		float halfWidth = 0;
		float halfLength = 0;
		float halfHeight = 0;
		std::vector<Vec3> vertices;
	public:
		AllignedBox() 
		{for (int i = 0; i < 8; i++) vertices.push_back(Vec3{0, 0, 0});}
		AllignedBox(Vec3 centerPos): centerPos(centerPos) 
		{for (int i = 0; i < 8; i++) vertices.push_back(centerPos);}
		AllignedBox(Vec3 centerPos, float width, float length, float height): centerPos(centerPos), halfLength(length/2.f), halfWidth(width/2.f), halfHeight(height/2.f) {};

		// Vector must be [x][y][z][x][y][z][x][...]
		AllignedBox(const std::vector<float>& vertices) 
		{
			float maxX = vertices[0], minX = vertices[0], 
				maxY = vertices[1], minY = vertices[1], 
				maxZ = vertices[2], minZ = vertices[2];
			for (int i = 0; i < vertices.size(); i+=3)
			{
				if (vertices[i] > maxX) maxX = vertices[i]; if (vertices[i] < minX) minX = vertices[i];
				if (vertices[i + 1] > maxY) maxY = vertices[i + 1]; if (vertices[i + 1] < minY) minY = vertices[i + 1];
				if (vertices[i + 2] > maxZ) maxZ = vertices[i + 2]; if (vertices[i + 2] < minZ) minZ = vertices[i + 2];
			}

			halfWidth = (maxX - minX) / 2;
			halfHeight = (maxY - minY) / 2;
			halfLength = (maxZ - minZ) / 2;

			centerPos.x = minX + halfWidth;
			centerPos.y = minY + halfHeight;
			centerPos.z = minZ + halfLength;
		}

	};
}