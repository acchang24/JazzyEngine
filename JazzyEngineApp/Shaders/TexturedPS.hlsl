#include "TexturedVS.hlsl"

float4 main(VertexOut psIn) : SV_TARGET
{
	return DiffuseTexture.Sample(DefaultSampler, psIn.uv);
}