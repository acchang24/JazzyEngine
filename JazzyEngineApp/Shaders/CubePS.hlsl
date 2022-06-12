#include "CubeVS.hlsl"

float4 main(uint tid : SV_PrimitiveID) : SV_TARGET
{
	return face_color[tid / 2];
}