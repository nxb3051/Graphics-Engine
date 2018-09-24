#include "Mesh.h"

Mesh::Mesh()
{
}

Mesh::Mesh(Vertex * vertArray, int numOfVerts, int * indexArray, int numOfInds)
{
	vertices = vertArray;
	vertexCount = numOfVerts;
	indices = indexArray;
	indexCount = numOfInds;
}

Mesh::~Mesh()
{
}

Vertex * Mesh::GetVertices()
{
	return vertices;
}

int Mesh::GetVertexCount()
{
	return vertexCount;
}

int * Mesh::GetIndices()
{
	return indices;
}

int Mesh::GetIndexCount()
{
	return indexCount;
}
