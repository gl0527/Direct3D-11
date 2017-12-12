#include "trafo.hlsli"

Texture2D kd;
SamplerState ss;

cbuffer matCap {
	float4x4 viewMat;
};

float4 psMatcap (VsosTrafo input) : SV_Target
{
	float4 camNorm = mul(float4 (normalize (input.worldNorm), 0.0f), viewMat);
	return kd.Sample(ss, 0.5f * camNorm.xy + float2 (0.5f, 0.5f));
}
