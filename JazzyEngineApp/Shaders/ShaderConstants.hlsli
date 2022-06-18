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

cbuffer ColorBuffer : register(b2)
{
	float4 face_color[6];
};

SamplerState DefaultSampler : register(s0);

Texture2D DiffuseTexture : register(t0);