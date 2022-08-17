#include "Real Engine/Components/PointLight.h"

#include "Real Engine/Components/Transform.h"

PointLight::PointLight(vec3 color) : Light(ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f), color)
{
}

PointLight::~PointLight()
{
}

LightInfos PointLight::GetLightInfos()
{
	return LightInfos(POINT_LIGHT, GetTransform()->position, lightColor, 1.0f, 0.09f, 0.032f, vec3(0.2, 0.2, 0.2), vec3(1, 1, 1), vec3(1, 1, 1), GetViewProjectionMatrix(), GetDepthMapBindlessHandle());
}