#include "trafo.hlsli"

cbuffer perFrame {
	float4 viewPos;
	float4 lightPos;
	float4 lightPowerDensity;
};

Texture2D kd;
TextureCube envTexture;
SamplerState linearSampler;

float4 psEnvMapped (VsosTrafo input) : SV_TARGET
{
	float3 reflDir = reflect (normalize (input.worldPos.xyz - viewPos.xyz), normalize (input.worldNorm));
	float envWeight = 1.0;

	return envWeight * envTexture.Sample (linearSampler, reflDir) + (1.0 - envWeight) * kd.Sample (linearSampler, input.tex);
}