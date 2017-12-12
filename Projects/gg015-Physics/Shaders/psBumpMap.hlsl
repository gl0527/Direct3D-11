#include "trafo.hlsli"

Texture2D kd;
Texture2D bump;
SamplerState ss;

cbuffer perFrame {
	float4 eyePos;
	float4 lightPos;
	float4 lightPowerDensity;
};

float4 psBumpMap (VsosTrafo input) : SV_Target{

	float4 ks = float4 (1.0f, 1.0f, 1.0f, 16.0f);
	float4 ka = float4 (0.1f, 0.1f, 0.1f, 1.0f);

	// we have to normalize the interpolated normal vector to ensure its unit length
	float3 n = normalize (input.worldNorm);

	// perturbate the normal vector with the bump map
	n = normalize (n + 0.9f * bump.Sample (ss, input.tex));

	float3 lightDir = normalize (lightPos.xyz - input.worldPos.xyz);
	float cosTheta = dot (n, lightDir);

	if (cosTheta < 0.0)
		return ka;

	float3 viewDir = normalize (eyePos.xyz - input.worldPos.xyz);
	float3 halfway = normalize (viewDir + lightDir);
	float cosDelta = dot (n, halfway);

	if (cosDelta < 0)
		return ka;

	float4 diffuseColor = kd.Sample (ss, input.tex) * lightPowerDensity * cosTheta;
	float4 specularColor = lightPowerDensity * ks * pow (cosDelta, ks.w);

	return ka + diffuseColor + specularColor;
}