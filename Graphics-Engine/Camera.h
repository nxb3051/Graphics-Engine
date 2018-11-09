#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <string>

#include "DXCore.h"

using namespace DirectX;

class Camera
{
private:
	XMFLOAT4X4 viewMatrix;
	XMFLOAT4X4 projectionMatrix;
	XMFLOAT4 position;
	XMFLOAT4 direction;
	XMFLOAT4 up;
	float xRotation;
	float yRotation;
	float movementSpeed = 3.0f;
	void DetectInput(float deltaTime);
public:
	Camera();
	~Camera();
	void Update(float deltaTime);
	void Rotate(float xAngle, float yAngle);
	XMFLOAT4X4 GetViewMatrix();
	XMFLOAT4X4 GetProjectionMatrix();
	XMFLOAT3 GetPosition();
	void SetProjectionMatrix(float width, float height);
};

