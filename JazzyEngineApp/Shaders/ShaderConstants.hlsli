#pragma pack_matrix(row_major)

cbuffer PerObjectConstants : register(b0)
{
	float4x4 modelToWorld;
}

cbuffer ColorBuffer : register(b1)
{
	float4 face_color[6];
};

SamplerState DefaultSampler : register(s0);

Texture2D DiffuseTexture : register(t0);