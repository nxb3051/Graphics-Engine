
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
};

struct DirectionalLight {
	float4 AmbientColor;
	float4 DiffuseColor;
	float3 Direction;
};

cbuffer Light : register( b1 ) {
	DirectionalLight light1;
	DirectionalLight light2;
};

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
	input.normal = normalize(input.normal);

	float3 light1Dir = normalize(-light1.Direction);
	float3 light2Dir = normalize(-light2.Direction);

	float NdotL1 = dot(input.normal, light1Dir);
	float NdotL2 = dot(input.normal, light2Dir);

	NdotL1 = saturate(NdotL1);
	NdotL2 = saturate(NdotL2);

	return light1.AmbientColor + light2.AmbientColor + (light1.DiffuseColor * NdotL1) + (light2.DiffuseColor * NdotL2);
}