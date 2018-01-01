﻿#include "CreateMesh.hpp"

namespace Flounder
{
	Model *CreateMesh::Create(const float &sideLength, const float &squareSize, const int &vertexCount, const float &textureScale, const MeshType &meshType, const std::function<float(float, float)> &getHeight, const std::function<Vector3(Vector3, Vector3)> &getColour)
	{
		std::vector<Vertex> vertices = std::vector<Vertex>();
		std::vector<uint32_t> indices = std::vector<uint32_t>();

		for (int col = 0; col < vertexCount; col++)
		{
			for (int row = 0; row < vertexCount; row++)
			{
				// Creates and stores vertices.
				Vector3 position = Vector3((row * squareSize) - (sideLength / 2.0f), 0.0f, (col * squareSize) - (sideLength / 2.0f));
				position.m_y = getHeight(position.m_x, position.m_z); // TODO: Simplify!

				const Vector2 uv = Vector2(
					textureScale * static_cast<float>(col) / static_cast<float>(vertexCount),
					textureScale * static_cast<float>(row) / static_cast<float>(vertexCount)
				);
				const Vector3 normal = CalculateNormal(position.m_x, position.m_z, 1.5f, getHeight);
				const Vector3 tangent = getColour(position, normal);

				vertices.push_back(Vertex(position, uv, normal, tangent));

				// Creates and stores indices.
				if (col < vertexCount - 1 && row < vertexCount - 1)
				{
					const uint32_t topLeft = (row * vertexCount) + col;
					const uint32_t topRight = topLeft + 1;
					const uint32_t bottomLeft = ((row + 1) * vertexCount) + col;
					const uint32_t bottomRight = bottomLeft + 1;

					switch (meshType)
					{
					case MeshSimple:
						GenerateIndicesSimple(topLeft, topRight, bottomLeft, bottomRight, indices);
						break;
					case MeshPattern:
						GenerateIndicesPattern(topLeft, topRight, bottomLeft, bottomRight, row % 2 == 0, col % 2 == 0, indices);
						break;
					}
				}
			}
		}

		return new Model(vertices, indices);
	}

	void CreateMesh::GenerateIndicesSimple(const uint32_t &topLeft, const uint32_t &topRight, const uint32_t &bottomLeft, const uint32_t &bottomRight, std::vector<uint32_t> &indices)
	{
		indices.push_back(topLeft);
		indices.push_back(bottomLeft);
		indices.push_back(bottomRight);
		indices.push_back(topLeft);
		indices.push_back(bottomRight);
		indices.push_back(topRight);
	}

	void CreateMesh::GenerateIndicesPattern(const uint32_t &topLeft, const uint32_t &topRight, const uint32_t &bottomLeft, const uint32_t &bottomRight, const bool &sign, const bool &mixed, std::vector<uint32_t> &indices)
	{
		if (sign)
		{
			indices.push_back(topLeft);
			indices.push_back(bottomLeft);
			indices.push_back(mixed ? topRight : bottomRight);
			indices.push_back(bottomRight);
			indices.push_back(topRight);
			indices.push_back(mixed ? bottomLeft : topLeft);
		}
		else
		{
			indices.push_back(topRight);
			indices.push_back(topLeft);
			indices.push_back(mixed ? bottomRight : bottomLeft);
			indices.push_back(bottomLeft);
			indices.push_back(bottomRight);
			indices.push_back(mixed ? topLeft : topRight);
		}
	}

	Vector3 CreateMesh::CalculateNormal(const float &x, const float &z, const float &squareSize, const std::function<float(float, float)> &getHeight)
	{
		const float heightL = getHeight(x - squareSize, z);
		const float heightR = getHeight(x + squareSize, z);
		const float heightD = getHeight(x, z - squareSize);
		const float heightU = getHeight(x, z + squareSize);

		Vector3 normal = Vector3(heightL - heightR, squareSize, heightD - heightU);
		normal.Normalize();
		return normal;
	}
}
