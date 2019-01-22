#pragma once

#include <DirectXMath.h>
#include <memory>

#include "WICTextureLoader.h"
#include "DDSTextureLoader.h"

#include "DXCore.h"
#include "SimpleShader.h"
#include "Mesh.h"
#include "GameEntity.h"
#include "Camera.h"
#include "Lights.h"

using namespace std;

class Game
	: public DXCore
{

public:
	Game(HINSTANCE hInstance);
	~Game();

	// Overridden setup and game loop methods, which
	// will be called automatically
	void Init();
	void OnResize();
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);

	// Overridden mouse input helper methods
	void OnMouseDown(WPARAM buttonState, int x, int y);
	void OnMouseUp(WPARAM buttonState, int x, int y);
	void OnMouseMove(WPARAM buttonState, int x, int y);
	void OnMouseWheel(float wheelDelta, int x, int y);
private:

	// Initialization helper methods - feel free to customize, combine, etc.
	void LoadShaders();
	void LoadTextures();
	void CreateMatrices();
	void CreateBasicGeometry();

	Mesh * sphereMesh;
	Mesh * cubeMesh;

	float rotateValue = 0.5f;

	ID3D11ShaderResourceView * lavaSRV;
	ID3D11ShaderResourceView * lavaNormal;
	ID3D11ShaderResourceView * crystalSRV;
	ID3D11ShaderResourceView * crystalNormal;
	ID3D11SamplerState * sState;
	D3D11_SAMPLER_DESC * sDescription;

	Material * lavaMat;
	Material * crystalMat;

	vector<GameEntity> shapes;

	Camera * cam;

	DirectionalLight firstLight;

	// Wrappers for DirectX shaders to provide simplified functionality
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

	//Sky
	ID3D11ShaderResourceView* skySRV;
	ID3D11DepthStencilState* skyDepthState;
	ID3D11RasterizerState* skyRasterizerState;

	Mesh* skyMesh;

	SimpleVertexShader* skyVertexShader;
	SimplePixelShader* skyPixelShader;

	// The matrices to go from model space to screen space
	DirectX::XMFLOAT4X4 worldMatrix;
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;

	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;
};

