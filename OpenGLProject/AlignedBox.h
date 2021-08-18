#pragma once

#include <vector>

#include "Vec3.h"

namespace fwork
{
	// GL_LINES
	const std::vector<unsigned int> AlignedBoxIndices =
	{
		0, 1, 1, 2, 2, 3, 3, 0,
		4, 5, 5, 6, 6, 7, 7, 4,
		0, 4, 1, 5, 2, 6, 3, 7
	};

	class AlignedBox
	{
		Vec3 centerPos;
		float halfWidth = 0;
		float halfLength = 0;
		float halfHeight = 0;
		std::vector<float> vertices;
	public:
		AlignedBox() 
		{for (int i = 0; i < 8*3; i++) vertices.push_back(0);}
		AlignedBox(Vec3 centerPos): centerPos(centerPos) 
		{
			for (int i = 0; i < 8; i++)
			{
				vertices.push_back(centerPos.x);
				vertices.push_back(centerPos.y);
				vertices.push_back(centerPos.z);
			}
		}
		AlignedBox(Vec3 centerPos, float width, float length, float height) 
		{
			init(centerPos, width, length, height);
		}

		// Example [x][y][z][][][x][y][z][][][x][...] - offset is 2
		AlignedBox(const std::vector<float>& vertices, int offset = 0)
		{
			initFromVertices(vertices, offset);
		}

		void init(Vec3 centerPos, float width, float length, float height)
		{
			this->centerPos = centerPos;
			this->halfWidth = width / 2.f;
			this->halfLength = length / 2.f;
			this->halfHeight = height / 2.f;

			generateVertices();
		}

		void initFromVertices(const std::vector<float>& vertices, int offset = 0)
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

			halfWidth = (maxX - minX) / 2;
			halfHeight = (maxY - minY) / 2;
			halfLength = (maxZ - minZ) / 2;

			centerPos.x = minX + halfWidth;
			centerPos.y = minY + halfHeight;
			centerPos.z = minZ + halfLength;

			generateVertices();
		}

		std::vector<float>& getVertices() { return vertices; }
		float getWidth() { return halfWidth*2; }
		float getHeight() { return halfHeight*2; }
		float getLength() { return halfLength*2; }
	private:
		
		void generateVertices()
		{
			vertices.clear();
			while (vertices.size() < 8 * 3) vertices.push_back(0.f);
			vertices[0 * 3] = vertices[3 * 3] = vertices[4 * 3] = vertices[7 * 3] = this->centerPos.x - halfWidth;
			vertices[1 * 3] = vertices[2 * 3] = vertices[6 * 3] = vertices[5 * 3] = this->centerPos.x + halfWidth;

			vertices[0 * 3 + 1] = vertices[1 * 3 + 1] = vertices[2 * 3 + 1] = vertices[3 * 3 + 1] = this->centerPos.y + halfHeight;
			vertices[4 * 3 + 1] = vertices[5 * 3 + 1] = vertices[6 * 3 + 1] = vertices[7 * 3 + 1] = this->centerPos.y - halfHeight;

			vertices[0 * 3 + 2] = vertices[1 * 3 + 2] = vertices[5 * 3 + 2] = vertices[4 * 3 + 2] = this->centerPos.z - halfHeight;
			vertices[3 * 3 + 2] = vertices[2 * 3 + 2] = vertices[6 * 3 + 2] = vertices[7 * 3 + 2] = this->centerPos.z + halfHeight;
		}
	};
}