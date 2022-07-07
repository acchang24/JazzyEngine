#include "SimpleLitVS.hlsl"

float4 main(VertexOut psIn) : SV_TARGET
{
	psIn.normal = normalize(psIn.normal);

	float3 diffuseLight;
	float3 specularLight;

	for (int i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		if (pointLight[i].isEnabled)
		{
			float3 lightToPixel = psIn.surface.xyz - pointLight[i].position;
			float lightToPixelDist = length(lightToPixel);

			if (lightToPixelDist >= pointLight[i].outerRadius)
			{
				lightToPixelDist = 0.0f;
			}
			else if (lightToPixelDist <= pointLight[i].innerRadius)
			{
				lightToPixelDist = 1.0f;
			}
			else
			{
				lightToPixelDist = smoothstep(pointLight[i].outerRadius, pointLight[i].innerRadius, lightToPixelDist);
			}


			float3 surfaceToLight = pointLight[i].position - psIn.surface.xyz;
			surfaceToLight = normalize(surfaceToLight);

			// Diffuse lighting
			float dotAnswer = dot(psIn.normal.xyz, surfaceToLight);
			dotAnswer = max(dotAnswer, 0.0f);
			diffuseLight += pointLight[i].lightColor * dotAnswer * lightToPixelDist;

			// Specular
			float3 reflectedLightDirection = reflect(-surfaceToLight, psIn.normal.xyz);
			float3 surfaceToViewer = camPosition - psIn.surface.xyz;
			surfaceToViewer = normalize(surfaceToViewer);

			float specDot = dot(reflectedLightDirection, surfaceToViewer);
			specDot = saturate(specDot);
			float powDot = pow(specDot, specularPower);
			specularLight += pointLight[i].lightColor * powDot * lightToPixelDist;

		}
	}

	float4 finalLight = float4(ambientColor + diffuseLight + specularLight, 1.0f);

	return finalLight;
}