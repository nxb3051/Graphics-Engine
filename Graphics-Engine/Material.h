#pragma once

#include <DirectXMath.h>

#include "DXCore.h"
#include "SimpleShader.h"

class Material
{
private:
	SimpleVertexShader * vertexShader;
	SimplePixelShader * pixelShader;
	ID3D11ShaderResourceView * srv;
	ID3D11SamplerState * sState;
public:
	Material();
	Material(SimpleVertexShader * vertShader, SimplePixelShader * pixShader, ID3D11ShaderResourceView * otherSRV, ID3D11SamplerState * otherState);
	SimpleVertexShader * GetVertexShader();
	SimplePixelShader * GetPixelShader();
	ID3D11ShaderResourceView * GetSRV();
	ID3D11SamplerState * GetSamplerState();
	~Material();
};

