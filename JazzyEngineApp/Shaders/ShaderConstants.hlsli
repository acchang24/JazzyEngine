cbuffer PerObjectConstants : register(b0)
{
	float4x4 modelToWorld;
}

cbuffer ColorBuffer : register(b1)
{
	float4 face_color[6];
};