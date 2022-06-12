#include "VertexShader.hlsl"

float4 main(VertexOut psIn) : SV_TARGET
{
	return psIn.color;
}