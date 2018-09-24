#pragma once

#include <d3d11.h>;
#include <DirectXMath.h>;
#include <memory>;
#include "Mesh.h";

using namespace DirectX;
using namespace std;

class GameEntity
{
private:
	ID3D11Buffer * vertexBuffer;
	ID3D11Buffer * indexBuffer;
	XMFLOAT4X4 worldMat;
	XMFLOAT3 position;
	XMFLOAT3 rotation;
	XMFLOAT3 scale;
	shared_ptr<Mesh> myMesh;
	bool changed;
public:
	GameEntity();
	GameEntity(shared_ptr<Mesh> mesh, ID3D11Device * directXDevice);
	~GameEntity();
	XMFLOAT4X4 GetWorldMat();
	XMFLOAT3 GetPosition();
	void SetPosition(XMFLOAT3 newPos);
	XMFLOAT3 GetRotation();
	void SetRotation(XMFLOAT3 newRot);
	void RotateZ(float angle);
	XMFLOAT3 GetScale();
	void SetScale(XMFLOAT3 newScale);
	void MoveLocalZ(float offset);
	void MoveLocalX(float offset);
	void CalculateWorldMat();
	void Draw(ID3D11DeviceContext * context);
};

