#pragma once

#include <d3d11.h>
#include "Vertex.h"
#include <memory>

using namespace std;

class Mesh
{
private:
	ID3D11Buffer * vertexBuffer;
	ID3D11Buffer * indexBuffer;
	Vertex * vertices;
	int vertexCount;
	int * indices;
	int indexCount;
public:
	Mesh();
	Mesh(Vertex * vertArray, int numOfVerts, int * indexArray, int numOfInds, ID3D11Device * directXDevice);
	~Mesh();
	ID3D11Buffer * GetVertexBuffer();
	ID3D11Buffer * GetIndexBuffer();
	Vertex * GetVertices();
	int GetVertexCount();
	int * GetIndices();
	int GetIndexCount();
};

