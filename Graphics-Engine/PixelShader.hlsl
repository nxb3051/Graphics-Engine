
// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage
struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
	float2 uv			: TEXCOORD;
};

struct DirectionalLight {
	float4 AmbientColor;
	float4 DiffuseColor;
	float3 Direction;
};

cbuffer Light : register( b1 ) {
	DirectionalLight light1;
};

Texture2D diffuseTexture : register(t0);
Texture2D normalTexture : register(t1);
SamplerState basicSampler : register(s0);

// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
float4 main(VertexToPixel input) : SV_TARGET
{
	float4 surfaceColor = diffuseTexture.Sample(basicSampler, input.uv);
	float3 normalmap = normalTexture.Sample(basicSampler, input.uv).rgb * 2 - 1;

	//find the true normal here and so on
	float3 normal = normalize(input.normal);
	float3 tangent = normalize(input.tangent - normal * dot(input.tangent, normal));
	float3 bitangent = cross(tangent, normal);
	float3x3 TBN = float3x3(tangent, bitangent, normal);

	float3 finalnormal = normalize(mul(normalmap, TBN));

	float3 light1Dir = normalize(-light1.Direction);

	float NdotL1 = dot(finalnormal, light1Dir);

	NdotL1 = saturate(NdotL1);

	float4 color1 = surfaceColor * (light1.AmbientColor + (light1.DiffuseColor * NdotL1));

	return  color1;
}