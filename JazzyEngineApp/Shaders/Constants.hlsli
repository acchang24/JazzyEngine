#pragma pack_matrix(row_major)

#define MAX_POINT_LIGHTS 10

struct PointLightData
{
	float3 lightColor;
	float3 position;
	float innerRadius;
	float outerRadius;
	bool isEnabled;
};

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

cbuffer LightingConstants : register(b3)
{
	float3 ambientColor;
	PointLightData pointLight[MAX_POINT_LIGHTS];
}

cbuffer ColorBuffer : register(b4)
{
	float4 face_color[6];
};

SamplerState DefaultSampler : register(s0);

Texture2D DiffuseTexture : register(t0);