#include "Constants.hlsli"

struct VertexIn
{
	float3 pos : POSITION;
	float2 uv : TEXCOORD;
};

struct VertexOut
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};

VertexOut main(VertexIn vIn)
{
	VertexOut output;

	output.pos = mul(float4(vIn.pos.x, vIn.pos.y, vIn.pos.z, 1.0f), modelToWorld);

	output.pos = mul(output.pos, viewProj);

	output.uv = vIn.uv;

	return output;
}