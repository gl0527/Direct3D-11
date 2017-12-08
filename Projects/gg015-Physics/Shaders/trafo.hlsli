// data to the vertex shader output stream
struct VsosTrafo
{
	float4 pos			: SV_POSITION;
	float4 worldPos 	: WORLDPOS;
	float3 normal 		: NORMAL;
	float2 tex 			: TEXCOORD;
};