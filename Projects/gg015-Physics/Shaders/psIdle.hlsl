#include "trafo.hlsli"

TextureCube envTexture;
Texture2D kd;
SamplerState ss;

cbuffer perFrame {
	float4 eyePos;
};

float4 psIdle(VsosTrafo input) : SV_Target{
	
	float3 reflDir = reflect(normalize(input.worldPos.xyz - eyePos.xyz), normalize(input.worldNorm));
	float wEnv = 0.3f;
	return envTexture.Sample(ss, reflDir) * wEnv + kd.Sample(ss, input.tex) * (1.0f - wEnv);
}