#pragma once

#include <DirectXMath.h>

#include "DXCore.h"
#include "SimpleShader.h"

class Material
{
private:
	SimpleVertexShader * vertexShader;
	SimplePixelShader * pixelShader;
	ID3D11ShaderResourceView * texture;
	ID3D11ShaderResourceView * normal;
	ID3D11SamplerState * sState;
public:
	Material();
	Material(SimpleVertexShader * vertShader, SimplePixelShader * pixShader, ID3D11ShaderResourceView * otherTexture, ID3D11ShaderResourceView * otherNormal, ID3D11SamplerState * otherState);
	SimpleVertexShader * GetVertexShader();
	SimplePixelShader * GetPixelShader();
	ID3D11ShaderResourceView * GetSRVTexture();
	ID3D11ShaderResourceView * GetSRVNormal();
	ID3D11SamplerState * GetSamplerState();
	~Material();
};

