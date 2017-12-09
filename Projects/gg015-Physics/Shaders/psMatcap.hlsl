#include "trafo.hlsli"

Texture2D kd;
SamplerState ss;

cbuffer matCap {
	float4x4 viewMat;
};

float4 psMatcap (VsosTrafo input) : SV_Target
{
	float4 normInCamSpace = mul(float4 (input.normal, 0.0f), viewMat);
	return kd.Sample(ss, 0.5f * normInCamSpace.xy + float2 (0.5f, 0.5f));
}
