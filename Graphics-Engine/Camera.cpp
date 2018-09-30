#include "Camera.h"

void Camera::DetectInput(float deltaTime)
{
	if (GetAsyncKeyState('W') & 0x8000) {
		XMVECTOR directionalMovement = XMLoadFloat4(&direction);
		directionalMovement = XMVectorScale(directionalMovement, movementSpeed);
		directionalMovement *= deltaTime;
		XMVECTOR pos = XMLoadFloat4(&position);
		pos = XMVector3Transform(pos, XMMatrixTranslationFromVector(directionalMovement));
		XMStoreFloat4(&position, pos);
	}
	if (GetAsyncKeyState('S') & 0x8000) {
		XMVECTOR directionalMovement = XMLoadFloat4(&direction);
		directionalMovement = XMVectorScale(directionalMovement, movementSpeed);
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

		right = XMVectorScale(right, movementSpeed);
		right *= -deltaTime;

		pos = XMVector3Transform(pos, XMMatrixTranslationFromVector(right));
		XMStoreFloat4(&position, pos);
	}
	if (GetAsyncKeyState('A') & 0x8000) {
		XMVECTOR pos = XMLoadFloat4(&position);
		XMVECTOR directionalMovement = XMLoadFloat4(&direction);
		XMVECTOR above = XMLoadFloat4(&up);

		XMVECTOR left = XMVector3Cross(directionalMovement, above);

		left = XMVectorScale(left, movementSpeed);
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

	xRotation = 3.14159265f;
	yRotation = 3.14159265f;
}

Camera::~Camera()
{
}

void Camera::Update(float deltaTime)
{
	DetectInput(deltaTime);
	XMVECTOR rotation = XMQuaternionRotationRollPitchYaw(xRotation, yRotation, 0.0f);
	XMFLOAT3 zDefault = XMFLOAT3(0.0f, 0.0f, 1.0f);
	XMVECTOR forward = XMLoadFloat3(&zDefault);

	forward = XMVector3Rotate(forward, rotation);

	XMMATRIX view = XMMatrixLookToLH(XMLoadFloat4(&position), forward, XMLoadFloat4(&up));

	XMStoreFloat4(&direction, forward);
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(view));
}

void Camera::Rotate(float xAngle, float yAngle)
{
	float rotate = xAngle * 3.14159265 / 180;
	xRotation -= rotate;
	if (xRotation > 3.14159265f) {
		xRotation = -3.14159265f;
	}
	else if (xRotation < -3.14159265f) {
		xRotation = 3.14159265f;
	}
	//char array[10];
	//sprintf(array, "%f", xRotation);
	//printf(array);
	//printf("\n");
	rotate = yAngle * 3.14159265 / 180;
	yRotation += rotate;
	if (yRotation > 3.14159265f) {
		yRotation = -3.14159265f;
	}
	else if (yRotation < -3.14159265f) {
		yRotation = 3.14159265f;
	}
}

XMFLOAT4X4 Camera::GetViewMatrix()
{
	return viewMatrix;
}

XMFLOAT4X4 Camera::GetProjectionMatrix()
{
	return projectionMatrix;
}

void Camera::SetProjectionMatrix(float width, float height)
{
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,	
		(float)width / height,
		0.1f,				
		100.0f);
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P));
}
