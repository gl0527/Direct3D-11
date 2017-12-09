#include "trafo.hlsli"

cbuffer perObject {
	row_major float4x4 modelMatrix;
	row_major float4x4 modelMatrixInverse;
	row_major float4x4 modelViewProjMatrix;
	row_major float4x4 rayDirMatrix;
};

struct IaosTrafo
{
	float4 modelPos		: POSITION;
	float3 modelNorm 	: NORMAL;
	float2 tex 			: TEXCOORD;
};

VsosTrafo vsTrafo(IaosTrafo input)
{
	VsosTrafo output = (VsosTrafo)0;

	// transforming the position from model space to normalized device coordinates
	output.ndcPos = mul(input.modelPos, modelViewProjMatrix);

	// calculating world position from model position
	output.worldPos = mul(input.modelPos, modelMatrix);

	// normal vector should be transformed with M^(-1)
	output.worldNorm = mul(modelMatrixInverse, float4(input.modelNorm, 0.0)).xyz;

	output.tex = input.tex;

	return output;
}
