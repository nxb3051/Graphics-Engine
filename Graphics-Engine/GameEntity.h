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
	XMFLOAT4X4 worldMat;
	XMFLOAT3 position;
	XMFLOAT3 rotation;
	XMFLOAT3 scale;
	Mesh * myMesh;
	bool changed;
public:
	GameEntity();
	GameEntity(Mesh * mesh);
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

