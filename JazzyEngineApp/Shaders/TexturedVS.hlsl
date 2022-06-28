#include "Constants.hlsli"

struct VertexIn
{
	float3 pos : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};

struct VertexOut
{
	float4 pos : SV_POSITION;
	float4 normal : NORMAL;
	float2 uv : TEXCOORD;
	float4 surface : SURFACE;
};

VertexOut main(VertexIn vIn)
{
	VertexOut output;

	output.pos = mul(float4(vIn.pos, 1.0f), modelToWorld);

	output.surface = mul(float4(vIn.pos, 1.0f), modelToWorld);

	output.pos = mul(output.pos, viewProj);

	output.uv = vIn.uv;

	output.normal = mul(float4(vIn.normal, 0.0f), modelToWorld);

	return output;
}