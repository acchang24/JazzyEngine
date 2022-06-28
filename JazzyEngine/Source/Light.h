#pragma once
#include "EngineMath.h"

namespace Light
{
	const int MAX_POINT_LIGHTS = 10;

	struct PointLightData
	{
		Vector3 lightColor;
		float pad1;
		Vector3 position;
		float innerRadius;
		float outerRadius;
		bool isEnabled;
		bool pad2;
		Vector2 pad3;
	};

	struct LightingConstants
	{
		Vector3 ambientColor;
		float pad;
		PointLightData pointLight[MAX_POINT_LIGHTS];
	};
}