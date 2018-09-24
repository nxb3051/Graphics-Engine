#pragma once

#include <d3d11.h>
#include "Vertex.h"
#include <memory>

using namespace std;

class Mesh
{
private:
	Vertex * vertices;
	int vertexCount;
	int * indices;
	int indexCount;
public:
	Mesh();
	Mesh(Vertex * vertArray, int numOfVerts, int * indexArray, int numOfInds);
	~Mesh();
	Vertex * GetVertices();
	int GetVertexCount();
	int * GetIndices();
	int GetIndexCount();
};

