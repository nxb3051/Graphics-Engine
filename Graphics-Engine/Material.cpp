#include "Material.h"



Material::Material()
{
	vertexShader = 0;
	pixelShader = 0;
}

Material::Material(SimpleVertexShader * vertShader, SimplePixelShader * pixShader)
{
	vertexShader = vertShader;
	pixelShader = pixShader;
}

SimpleVertexShader * Material::GetVertexShader()
{
	return vertexShader;
}

SimplePixelShader * Material::GetPixelShader()
{
	return pixelShader;
}


Material::~Material()
{
}
