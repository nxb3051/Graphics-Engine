#include "Material.h"



Material::Material()
{
	vertexShader = 0;
	pixelShader = 0;
	srv = 0;
	sState = 0;
}

Material::Material(SimpleVertexShader * vertShader, SimplePixelShader * pixShader, ID3D11ShaderResourceView * otherSRV, ID3D11SamplerState * otherState)
{
	vertexShader = vertShader;
	pixelShader = pixShader;
	srv = otherSRV;
	sState = otherState;
}

SimpleVertexShader * Material::GetVertexShader()
{
	return vertexShader;
}

SimplePixelShader * Material::GetPixelShader()
{
	return pixelShader;
}

ID3D11ShaderResourceView * Material::GetSRV()
{
	return srv;
}

ID3D11SamplerState * Material::GetSamplerState()
{
	return sState;
}


Material::~Material()
{
}
