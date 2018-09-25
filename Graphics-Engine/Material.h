#pragma once

#include <DirectXMath.h>

#include "DXCore.h"
#include "SimpleShader.h"

class Material
{
private:
	SimpleVertexShader * vertexShader;
	SimplePixelShader * pixelShader;
public:
	Material();
	Material(SimpleVertexShader * vertShader, SimplePixelShader * pixShader);
	SimpleVertexShader * GetVertexShader();
	SimplePixelShader * GetPixelShader();
	~Material();
};

