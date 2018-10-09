#include "GameEntity.h"

GameEntity::GameEntity()
{
	position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	XMStoreFloat4x4(&worldMat, XMMatrixIdentity());
	myMesh = nullptr;
	changed = false;
}

//Sets default values for transformation vectors and world matrix
GameEntity::GameEntity(Mesh * mesh, Material * material)
{
	position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	XMStoreFloat4x4(&worldMat, XMMatrixIdentity());
	myMesh = mesh;
	myMaterial = material;

	changed = false;
}

//Clears allocated memory for pointers
GameEntity::~GameEntity()
{
	myMesh = nullptr;
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

void GameEntity::Rotate(float xAngle, float yAngle, float zAngle)
{
	rotation = XMFLOAT3(rotation.x + xAngle, rotation.y + yAngle, rotation.z + zAngle);
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

void GameEntity::PrepareMaterial(XMFLOAT4X4 viewMat, XMFLOAT4X4 projMat)
{
	CalculateWorldMat();

	SimpleVertexShader * tempVertShader = myMaterial->GetVertexShader();

	tempVertShader->SetMatrix4x4("world", worldMat);
	tempVertShader->SetMatrix4x4("view", viewMat);
	tempVertShader->SetMatrix4x4("projection", projMat);

	tempVertShader->CopyAllBufferData();

	tempVertShader->SetShader();

	SimplePixelShader * tempPixelShader = myMaterial->GetPixelShader();

	tempPixelShader->SetSamplerState("basicSampler", myMaterial->GetSamplerState());
	tempPixelShader->SetShaderResourceView("diffuseTexture", myMaterial->GetSRVTexture());
	tempPixelShader->SetShaderResourceView("normalTexture", myMaterial->GetSRVNormal());
	myMaterial->GetPixelShader()->SetShader();
}

void GameEntity::Draw(ID3D11DeviceContext * context)
{
	if (myMesh != nullptr) {
		UINT stride = sizeof(Vertex);
		UINT offset = 0;

		ID3D11Buffer * tempVertexBuffer = myMesh->GetVertexBuffer();

		context->IASetVertexBuffers(0, 1, &tempVertexBuffer, &stride, &offset);
		context->IASetIndexBuffer(myMesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
		context->DrawIndexed(myMesh->GetIndexCount(), 0, 0);
	}
}
