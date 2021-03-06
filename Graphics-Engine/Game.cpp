#include "Game.h"
#include "Vertex.h"

// For the DirectX Math library
using namespace DirectX;

// --------------------------------------------------------
// Constructor
//
// DXCore (base class) constructor will set up underlying fields.
// DirectX itself, and our window, are not ready yet!
//
// hInstance - the application's OS-level handle (unique ID)
// --------------------------------------------------------
Game::Game(HINSTANCE hInstance)
	: DXCore(
		hInstance,		   // The application's handle
		"DirectX Game",	   // Text for the window's title bar
		1280,			   // Width of the window's client area
		720,			   // Height of the window's client area
		true)			   // Show extra stats (fps) in title bar?
{
	// Initialize fields
	vertexShader = 0;
	pixelShader = 0;
	sphereMesh = 0;
	cubeMesh = 0;
	lavaSRV = 0;
	lavaNormal = 0;
	crystalSRV = 0;
	crystalNormal = 0;
	sState = 0;
	sDescription = 0;
	lavaMat = 0;
	crystalMat = 0;
	cam = new Camera();

#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.");
#endif
	
}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Release all DirectX objects created here
//  - Delete any objects to prevent memory leaks
// --------------------------------------------------------
Game::~Game()
{
	// Delete our simple shader objects, which
	// will clean up their own internal DirectX stuff
	delete vertexShader;
	delete pixelShader;
	delete sphereMesh;
	delete cubeMesh;
	delete lavaMat;
	delete crystalMat;
	lavaSRV->Release();
	lavaNormal->Release();
	crystalSRV->Release();
	crystalNormal->Release();
	sState->Release();
	delete sDescription;
	delete cam;

	delete skyPixelShader;
	delete skyVertexShader;
	delete skyMesh;

	skySRV->Release();
	skyRasterizerState->Release();
	skyDepthState->Release();
}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
	//Sampler Description Field Initialization
	sDescription = new D3D11_SAMPLER_DESC();
	sDescription->AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sDescription->AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sDescription->AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sDescription->BorderColor[0] = 0.0f;
	sDescription->BorderColor[1] = 0.0f;
	sDescription->BorderColor[2] = 0.0f;
	sDescription->BorderColor[3] = 0.0f;
	sDescription->ComparisonFunc = D3D11_COMPARISON_NEVER;
	sDescription->Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sDescription->MaxAnisotropy = 0;
	sDescription->MaxLOD = D3D11_FLOAT32_MAX;
	sDescription->MinLOD = 0;
	sDescription->MipLODBias = 0;

	device->CreateSamplerState(sDescription, &sState);
	LoadTextures();

	// Helper methods for loading shaders, creating some basic
	// geometry to draw and some simple camera matrices.
	//  - You'll be expanding and/or replacing these later	
	LoadShaders();
	CreateMatrices();
	cam->SetProjectionMatrix(width, height);

	CreateBasicGeometry();

	firstLight.AmbientColor = XMFLOAT4(0.1, 0.1, 0.1, 1.0);
	firstLight.DiffuseColor = XMFLOAT4(1, 1, 1, 1);
	firstLight.Direction = XMFLOAT3(1, -1, 0);

	//SkyBox
	D3D11_RASTERIZER_DESC rs = {};
	rs.FillMode = D3D11_FILL_SOLID;
	rs.CullMode = D3D11_CULL_FRONT;
	rs.DepthClipEnable = true;
	device->CreateRasterizerState(&rs, &skyRasterizerState);

	D3D11_DEPTH_STENCIL_DESC ds = {};
	ds.DepthEnable = true;
	ds.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	ds.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	device->CreateDepthStencilState(&ds, &skyDepthState);

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.  
	// Essentially: "What kind of shape should the GPU draw with our data?"
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files using
// my SimpleShader wrapper for DirectX shader manipulation.
// - SimpleShader provides helpful methods for sending
//   data to individual variables on the GPU
// --------------------------------------------------------
void Game::LoadShaders()
{
	vertexShader = new SimpleVertexShader(device, context);
	vertexShader->LoadShaderFile(L"VertexShader.cso");

	pixelShader = new SimplePixelShader(device, context);
	pixelShader->LoadShaderFile(L"PixelShader.cso");

	skyVertexShader = new SimpleVertexShader(device, context);
	skyVertexShader->LoadShaderFile(L"SkyBoxVS.cso");

	skyPixelShader = new SimplePixelShader(device, context);
	skyPixelShader->LoadShaderFile(L"SkyBoxPS.cso");

	lavaMat = new Material(vertexShader, pixelShader, lavaSRV, lavaNormal, sState);
	crystalMat = new Material(vertexShader, pixelShader, crystalSRV, crystalNormal, sState);
}

void Game::LoadTextures()
{
	CreateWICTextureFromFile(device, context, L"../../Assets/Textures/Lava_005_COLOR.jpg", 0, &lavaSRV);
	CreateWICTextureFromFile(device, context, L"../../Assets/Textures/Lava_005_NORM.jpg", 0, &lavaNormal);
	CreateWICTextureFromFile(device, context, L"../../Assets/Textures/Sapphire_001_COLOR.jpg", 0, &crystalSRV);
	CreateWICTextureFromFile(device, context, L"../../Assets/Textures/Sapphire_001_NORM.jpg", 0, &crystalNormal);
	CreateDDSTextureFromFile(device, context, L"../../Assets/Textures/Sky/SunnyCubeMap.dds", 0, &skySRV);
}

// --------------------------------------------------------
// Initializes the matrices necessary to represent our geometry's 
// transformations and our 3D camera
// --------------------------------------------------------
void Game::CreateMatrices()
{
	// Set up world matrix
	// - In an actual game, each object will need one of these and they should
	//    update when/if the object moves (every frame)
	// - You'll notice a "transpose" happening below, which is redundant for
	//    an identity matrix.  This is just to show that HLSL expects a different
	//    matrix (column major vs row major) than the DirectX Math library
	XMMATRIX W = XMMatrixIdentity();
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(W)); // Transpose for HLSL!

	// Create the View matrix
	// - In an actual game, recreate this matrix every time the camera 
	//    moves (potentially every frame)
	// - We're using the LOOK TO function, which takes the position of the
	//    camera and the direction vector along which to look (as well as "up")
	// - Another option is the LOOK AT function, to look towards a specific
	//    point in 3D space
	XMVECTOR pos = XMVectorSet(0, 0, -5, 0);
	XMVECTOR dir = XMVectorSet(0, 0, 1, 0);
	XMVECTOR up = XMVectorSet(0, 1, 0, 0);
	XMMATRIX V = XMMatrixLookToLH(
		pos,     // The position of the "camera"
		dir,     // Direction the camera is looking
		up);     // "Up" direction in 3D space (prevents roll)
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V)); // Transpose for HLSL!

	// Create the Projection matrix
	// - This should match the window's aspect ratio, and also update anytime
	//    the window resizes (which is already happening in OnResize() below)
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,		// Field of View Angle
		(float)width / height,		// Aspect ratio
		0.1f,						// Near clip plane distance
		100.0f);					// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
}


// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void Game::CreateBasicGeometry()
{
	/*
	// Create some temporary variables to represent colors
	// - Not necessary, just makes things more readable
	XMFLOAT4 red = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 green = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	XMFLOAT4 blue = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	XMFLOAT4 yellow = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
	XMFLOAT4 purple = XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f);
	XMFLOAT4 orange = XMFLOAT4(1.0f, 0.5f, 0.0f, 1.0f);

	Vertex triVertices[] =
	{
		{ XMFLOAT3(-0.0f, +1.0f, +0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(+1.0f, -1.0f, +0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, +0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
	};

	Vertex squVertices[] =
	{
		{ XMFLOAT3(-1.0f, +1.0f, +0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(+1.0f, +1.0f, +0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(+1.0f, -1.0f, +0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, +0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
	};

	UINT triIndices[] = { 0, 1, 2 };

	UINT squIndices[] = { 0, 1, 2, 0, 2, 3 };
	*/

	sphereMesh = new Mesh("../../Assets/Models/sphere.obj", device);
	cubeMesh = new Mesh("../../Assets/Models/cube.obj", device);

	skyMesh = new Mesh("../../Assets/Models/cube.obj", device);

	GameEntity helixOne = GameEntity(sphereMesh, lavaMat);
	GameEntity cubeOne = GameEntity(cubeMesh, crystalMat);

	cubeOne.MoveLocalX(2.0f);

	shapes.push_back(helixOne);
	shapes.push_back(cubeOne);
}


// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void Game::OnResize()
{
	// Handle base-level DX resize stuff
	DXCore::OnResize();

	cam->SetProjectionMatrix(width, height);
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();

	if (GetAsyncKeyState('R')) {
		rotateValue *= -1;
	}

	cam->Update(deltaTime);

	if (shapes.size() > 0) {
		shapes[0].Rotate(0.0f, 0.5f * deltaTime, 0.0f);
		shapes[1].Rotate(rotateValue * deltaTime, rotateValue * deltaTime, 0.0f);
	}
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };

	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of Draw (before drawing *anything*)
	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(
		depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	// Send data to shader variables
	//  - Do this ONCE PER OBJECT you're drawing
	//  - This is actually a complex process of copying data to a local buffer
	//    and then copying that entire buffer to the GPU.  
	//  - The "SimpleShader" class handles all of that for you.

	for (int i = 0; i < shapes.size(); i++) {
		pixelShader->SetData("light1", &firstLight, sizeof(DirectionalLight));
		pixelShader->SetFloat3("cameraPosition", cam->GetPosition());
		pixelShader->CopyAllBufferData();
		shapes[i].PrepareMaterial(cam->GetViewMatrix(), cam->GetProjectionMatrix());
		pixelShader->SetShaderResourceView("SkyTex", skySRV);
		shapes[i].Draw(context);
	}

	//SKYYYYYYYYBOXXXXXX
	context->RSSetState(skyRasterizerState);
	context->OMSetDepthStencilState(skyDepthState, 0);

	// After drawing all of our regular (solid) objects, draw the sky!
	ID3D11Buffer* skyVB = skyMesh->GetVertexBuffer();
	ID3D11Buffer* skyIB = skyMesh->GetIndexBuffer();

	// Set the buffers
	context->IASetVertexBuffers(0, 1, &skyVB, &stride, &offset);
	context->IASetIndexBuffer(skyIB, DXGI_FORMAT_R32_UINT, 0);

	skyVertexShader->SetMatrix4x4("view", cam->GetViewMatrix());
	skyVertexShader->SetMatrix4x4("projection", cam->GetProjectionMatrix());

	skyVertexShader->CopyAllBufferData();
	skyVertexShader->SetShader();

	// Send texture-related stuff
	skyPixelShader->SetShaderResourceView("SkyTex", skySRV);
	skyPixelShader->SetSamplerState("SkySampler", sState);

	skyPixelShader->CopyAllBufferData(); // Remember to copy to the GPU!!!!
	skyPixelShader->SetShader();

	// Finally do the actual drawing
	context->DrawIndexed(skyMesh->GetIndexCount(), 0, 0);

	//Reset changed states
	context->RSSetState(0);
	context->OMSetDepthStencilState(0, 0);

	// Present the back buffer to the user
	//  - Puts the final frame we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
	swapChain->Present(0, 0);
}


#pragma region Mouse Input

// --------------------------------------------------------
// Helper method for mouse clicking.  We get this information
// from the OS-level messages anyway, so these helpers have
// been created to provide basic mouse input if you want it.
// --------------------------------------------------------
void Game::OnMouseDown(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;

	// Caputure the mouse so we keep getting mouse move
	// events even if the mouse leaves the window.  we'll be
	// releasing the capture once a mouse button is released
	SetCapture(hWnd);
}

// --------------------------------------------------------
// Helper method for mouse release
// --------------------------------------------------------
void Game::OnMouseUp(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// We don't care about the tracking the cursor outside
	// the window anymore (we're not dragging if the mouse is up)
	ReleaseCapture();
}

// --------------------------------------------------------
// Helper method for mouse movement.  We only get this message
// if the mouse is currently over the window, or if we're 
// currently capturing the mouse.
// --------------------------------------------------------
void Game::OnMouseMove(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	float yAngle = x - prevMousePos.x;
	float xAngle = y - prevMousePos.y;
	cam->Rotate(xAngle, yAngle);

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;
}

// --------------------------------------------------------
// Helper method for mouse wheel scrolling.  
// WheelDelta may be positive or negative, depending 
// on the direction of the scroll
// --------------------------------------------------------
void Game::OnMouseWheel(float wheelDelta, int x, int y)
{
	// Add any custom code here...
}
#pragma endregion