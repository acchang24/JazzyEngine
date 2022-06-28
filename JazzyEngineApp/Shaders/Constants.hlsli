#pragma pack_matrix(row_major)

cbuffer PerObjectConstants : register(b0)
{
	float4x4 modelToWorld;
}

cbuffer CameraConstants : register(b1)
{
	float4x4 viewProj;
	float3 camPosition;
}

cbuffer MaterialConstants: register(b2)
{
	float3 diffuseColor;
	float3 specularColor;
	float specularPower;
}

cbuffer ColorBuffer : register(b3)
{
	float4 face_color[6];
};

SamplerState DefaultSampler : register(s0);

Texture2D DiffuseTexture : register(t0);