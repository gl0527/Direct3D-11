// data to the vertex shader output stream
struct VsosTrafo
{
	float4 ndcPos		: SV_POSITION;
	float4 worldPos 	: WORLDPOS;
	float3 worldNorm 	: NORMAL;
	float2 tex 			: TEXCOORD;
};