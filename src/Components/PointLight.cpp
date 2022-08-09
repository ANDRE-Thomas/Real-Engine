#include "Components/PointLight.h"

#include "Components/Transform.h"

PointLight::PointLight(vec3 color) : Light(color)
{
}

PointLight::~PointLight()
{
}

LightInfos PointLight::GetLightInfos()
{
	return LightInfos(POINT_LIGHT, GetTransform()->position, lightColor, 1, 0.09, 0.032, vec3(0.2, 0.2, 0.2), vec3(1, 1, 1), vec3(1, 1, 1));
}