
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
	float3 worldPos		: POSITION;
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

cbuffer Camera : register(b2) {
	float3 cameraPosition;
}

Texture2D diffuseTexture : register(t0);
Texture2D normalTexture : register(t1);
TextureCube SkyTex : register(t2);
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

	input.normal = normalize(input.normal);
	input.tangent = normalize(input.tangent);

	//find the true normal here and so on
	input.tangent = normalize(input.tangent - input.normal * dot(input.tangent, input.normal));
	float3 bitangent = cross(input.tangent, input.normal);
	float3x3 TBN = float3x3(input.tangent, bitangent, input.normal);

	float3 finalnormal = normalize(mul(normalmap, TBN));

	float3 light1Dir = normalize(-light1.Direction);

	float NdotL1 = dot(finalnormal, light1Dir);

	NdotL1 = saturate(NdotL1);

	float3 reflection = reflect(-light1Dir, finalnormal);
	float3 dirToCamera = normalize(cameraPosition - input.worldPos);
	float specAmt = pow(saturate(dot(reflection, dirToCamera)), 64.0f);
	float4 specColor = float4(1, 1, 1, 1) * specAmt;

	float3 reflectVector = reflect(-dirToCamera, finalnormal);
	float4 reflectColor = SkyTex.Sample(basicSampler, reflectVector);

	float4 color1 = surfaceColor * (light1.AmbientColor + (light1.DiffuseColor * NdotL1) + specColor);

	float4 baseReflectColor = float4(0.3, 0.3, 0.3, 1.0);

	color1 += baseReflectColor*reflectColor;

	return color1;
}