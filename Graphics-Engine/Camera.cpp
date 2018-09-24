#include "Camera.h"

void Camera::DetectInput(float deltaTime)
{
	if (GetAsyncKeyState('W') & 0x8000) {
		XMVECTOR directionalMovement = XMLoadFloat4(&direction);
		directionalMovement *= deltaTime;
		XMVECTOR pos = XMLoadFloat4(&position);
		pos = XMVector3Transform(pos, XMMatrixTranslationFromVector(directionalMovement));
		XMStoreFloat4(&position, pos);
	}
	if (GetAsyncKeyState('S') & 0x8000) {
		XMVECTOR directionalMovement = XMLoadFloat4(&direction);
		directionalMovement *= -deltaTime;
		XMVECTOR pos = XMLoadFloat4(&position);
		pos = XMVector3Transform(pos, XMMatrixTranslationFromVector(directionalMovement));
		XMStoreFloat4(&position, pos);
	}
	if (GetAsyncKeyState('D') & 0x8000) {
		XMVECTOR pos = XMLoadFloat4(&position);
		XMVECTOR directionalMovement = XMLoadFloat4(&direction);
		XMVECTOR above = XMLoadFloat4(&up);

		XMVECTOR right = XMVector3Cross(directionalMovement, above);

		right *= -deltaTime;

		pos = XMVector3Transform(pos, XMMatrixTranslationFromVector(right));
		XMStoreFloat4(&position, pos);
	}
	if (GetAsyncKeyState('A') & 0x8000) {
		XMVECTOR pos = XMLoadFloat4(&position);
		XMVECTOR directionalMovement = XMLoadFloat4(&direction);
		XMVECTOR above = XMLoadFloat4(&up);

		XMVECTOR left = XMVector3Cross(directionalMovement, above);

		left *= deltaTime;

		pos = XMVector3Transform(pos, XMMatrixTranslationFromVector(left));
		XMStoreFloat4(&position, pos);
	}
	if (GetAsyncKeyState(' ') & 0x8000) {
		position.y += movementSpeed * deltaTime;
	}
	if (GetAsyncKeyState('X') & 0x8000) {
		position.y -= movementSpeed * deltaTime;
	}
}

Camera::Camera()
{
	XMStoreFloat4x4(&viewMatrix, XMMatrixIdentity());

	position = XMFLOAT4(0.0f, 0.0f, -5.0f, 0.0f);
	direction = XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f);
	up = XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f);

	xRotation = 0.0f;
	zRotation = 0.0f;
}

Camera::~Camera()
{
}

void Camera::Update(float deltaTime)
{
	DetectInput(deltaTime);
	XMVECTOR rotation = XMQuaternionRotationRollPitchYaw(xRotation, 0.0f, zRotation);
	XMFLOAT3 zDefault = XMFLOAT3(0.0f, 0.0f, 1.0f);
	XMVECTOR forward = XMLoadFloat3(&zDefault);

	forward = XMVector3Rotate(forward, rotation);

	XMMATRIX view = XMMatrixLookToLH(XMLoadFloat4(&position), XMLoadFloat4(&direction), XMLoadFloat4(&up));

	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(view));
}

XMFLOAT4X4 Camera::GetViewMatrix()
{
	return XMFLOAT4X4();
}
