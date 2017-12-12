#include "trafo.hlsli"

cbuffer perFrame {
	float4 viewPos;
	float4 lightPos;
	float4 lightPowerDensity;
};

TextureCube envTexture;
SamplerState linearSampler;


float3 Fresnel (float3 idxOfRefr, float3 absorptCoeff, float3 normal, float3 toEye)
{
	float3 whitef3 = float3 (1.0f, 1.0f, 1.0f);
	
	float3 num = pow (idxOfRefr - whitef3, 2.0f) + pow (absorptCoeff, 2.0f);
	float3 den = pow (idxOfRefr + whitef3, 2.0f) + pow (absorptCoeff, 2.0f);
	float3 F0 = num / den;

	float cosa = abs (dot (normal, toEye));
	
	return F0 + (whitef3 - F0) * pow (1.0f - cosa, 5.0f);
}


float4 psGlass (VsosTrafo input) : SV_TARGET
{
	// https://digitalerr0r.wordpress.com/2009/05/03/xna-shader-programming-tutorial-16-refraction/
	float3 rayDir = normalize (input.worldPos.xyz - viewPos.xyz);
	float3 norm = normalize (input.worldNorm);
	
	float3 reflDir = reflect (rayDir, norm);
	float3 refrDir = refract (rayDir, norm, 0.6581f);	// 0.6581f = refrIdx_of_air / refrIdx_of_glass
	float3 fresnel = Fresnel (float3 (1.52f, 1.52f, 1.52f), float3 (0.0f, 0.0f, 0.0f), norm, -rayDir);
	
	float3 outColor = fresnel * envTexture.Sample (linearSampler, reflDir).xyz + 
		(float3 (1.0f, 1.0f, 1.0f) - fresnel) * envTexture.Sample (linearSampler, refrDir).xyz;

	return float4 (outColor, 1.0f);
}
