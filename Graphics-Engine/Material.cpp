#include "Material.h"



Material::Material()
{
	vertexShader = 0;
	pixelShader = 0;
	texture = 0;
	sState = 0;
}

Material::Material(SimpleVertexShader * vertShader, SimplePixelShader * pixShader, ID3D11ShaderResourceView * otherTexture, ID3D11ShaderResourceView * otherNormal, ID3D11SamplerState * otherState)
{
	vertexShader = vertShader;
	pixelShader = pixShader;
	texture = otherTexture;
	normal = otherNormal;
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

ID3D11ShaderResourceView * Material::GetSRVTexture()
{
	return texture;
}

ID3D11ShaderResourceView * Material::GetSRVNormal()
{
	return normal;
}

ID3D11SamplerState * Material::GetSamplerState()
{
	return sState;
}


Material::~Material()
{
}
