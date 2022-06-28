#include "TexturedVS.hlsl"

float4 main(VertexOut psIn) : SV_TARGET
{
	return DiffuseTexture.Sample(DefaultSampler, psIn.uv);
	//return float4(pointLight[0].lightColor, 1.0f);
}