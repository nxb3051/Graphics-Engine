#include "GameEntity.h"

GameEntity::GameEntity()
{
	position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	XMStoreFloat4x4(&worldMat, XMMatrixIdentity());
	myMesh = nullptr;
	vertexBuffer = nullptr;
	indexBuffer = nullptr;
	changed = false;
}

//Sets default values for transformation vectors and world matrix
GameEntity::GameEntity(shared_ptr<Mesh> mesh, ID3D11Device * directXDevice)
{
	position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	XMStoreFloat4x4(&worldMat, XMMatrixIdentity());
	myMesh = mesh;

	//Vertex Buffer
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * myMesh.get()->GetVertexCount();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initialVertexData;
	initialVertexData.pSysMem = myMesh.get()->GetVertices();

	directXDevice->CreateBuffer(&vbd, &initialVertexData, &vertexBuffer);

	//Index Buffer
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(int) * myMesh.get()->GetIndexCount();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initialIndexData;
	initialIndexData.pSysMem = myMesh.get()->GetIndices();

	directXDevice->CreateBuffer(&ibd, &initialIndexData, &indexBuffer);

	changed = false;
}

//Clears allocated memory for pointers
GameEntity::~GameEntity()
{
	myMesh.reset();
}

//Returns the entity's world matrix
XMFLOAT4X4 GameEntity::GetWorldMat()
{
	return worldMat;
}

//Returns the entity's position
XMFLOAT3 GameEntity::GetPosition()
{
	return position;
}

//Sets the entity's position
void GameEntity::SetPosition(XMFLOAT3 newPos)
{
	position = newPos;
	changed = true;
}

//Returns the entity's rotation
XMFLOAT3 GameEntity::GetRotation()
{
	return rotation;
}

//Sets the entity's rotation
void GameEntity::SetRotation(XMFLOAT3 newRot)
{
	rotation = newRot;
	changed = true;
}

void GameEntity::RotateZ(float angle)
{
	rotation = XMFLOAT3(rotation.x, rotation.y, rotation.z + angle);
	changed = true;
}

//Returns the entity's scale
XMFLOAT3 GameEntity::GetScale()
{
	return scale;
}

//Sets the entity's scale
void GameEntity::SetScale(XMFLOAT3 newScale)
{
	scale = newScale;
	changed = true;
}

//Moves the enemy forward in it's local axis.
//Negative numbers move the entity backward.
void GameEntity::MoveLocalZ(float offset)
{
	position.z -= offset;
	changed = true;
}

//Moves the enemy sideways in it's local axiz.
//Positive numbers will move it to the right.
//Negative numbers move the entity left.
void GameEntity::MoveLocalX(float offset)
{
	position.x += offset;
	changed = true;
}

void GameEntity::CalculateWorldMat()
{
	if (changed) {
		XMMATRIX scaling = XMMatrixScaling(scale.x, scale.y, scale.z);
		XMMATRIX rotating = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
		XMMATRIX translation = XMMatrixTranslation(position.x, position.y, position.z);
		XMStoreFloat4x4(&worldMat, XMMatrixTranspose((scaling * rotating) * translation));
		changed = false;
	}
}

void GameEntity::Draw(ID3D11DeviceContext * context)
{
	if (myMesh != nullptr) {
		UINT stride = sizeof(Vertex);
		UINT offset = 0;

		context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		context->DrawIndexed(myMesh.get()->GetIndexCount(), 0, 0);
	}
}
