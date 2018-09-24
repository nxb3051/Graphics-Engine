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
	XMFLOAT4 position;
	XMFLOAT4 direction;
	XMFLOAT4 up;
	float xRotation;
	float zRotation;
	float movementSpeed = 1.0f;
	void DetectInput(float deltaTime);
public:
	Camera();
	~Camera();
	void Update(float deltaTime);
	XMFLOAT4X4 GetViewMatrix();
};

