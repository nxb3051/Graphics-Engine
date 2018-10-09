#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <fstream>
#include <vector>

#include "Vertex.h"

using namespace std;
using namespace DirectX;

class Mesh
{
private:
	ID3D11Buffer * vertexBuffer;
	ID3D11Buffer * indexBuffer;
	Vertex * vertices;
	int vertexCount;
	UINT * indices;
	int indexCount;
public:
	Mesh();
	Mesh(char* fileName, ID3D11Device * directXDevice);
	Mesh(Vertex * vertArray, int numOfVerts, UINT * indexArray, int numOfInds, ID3D11Device * directXDevice);
	void init(Vertex * vertArray, int numOfVerts, UINT * indexArray, int numOfInds, ID3D11Device * directXDevice);
	~Mesh();
	void ComputeTangents();
	ID3D11Buffer * GetVertexBuffer();
	ID3D11Buffer * GetIndexBuffer();
	Vertex * GetVertices();
	int GetVertexCount();
	UINT * GetIndices();
	int GetIndexCount();
};

