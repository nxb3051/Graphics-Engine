#include "Mesh.h"

Mesh::Mesh()
{
	vertexBuffer = nullptr;
	indexBuffer = nullptr;
}

Mesh::Mesh(Vertex * vertArray, int numOfVerts, int * indexArray, int numOfInds, ID3D11Device * directXDevice)
{
	vertices = vertArray;
	vertexCount = numOfVerts;
	indices = indexArray;
	indexCount = numOfInds;
	
	//Vertex Buffer
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * vertexCount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initialVertexData;
	initialVertexData.pSysMem = vertices;

	directXDevice->CreateBuffer(&vbd, &initialVertexData, &vertexBuffer);

	//Index Buffer
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(int) * indexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initialIndexData;
	initialIndexData.pSysMem = indices;

	directXDevice->CreateBuffer(&ibd, &initialIndexData, &indexBuffer);
}

Mesh::~Mesh()
{
	vertexBuffer->Release();
	indexBuffer->Release();
}

ID3D11Buffer * Mesh::GetVertexBuffer()
{
	return vertexBuffer;
}

ID3D11Buffer * Mesh::GetIndexBuffer()
{
	return indexBuffer;
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
