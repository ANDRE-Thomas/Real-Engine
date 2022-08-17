#include "Real Engine/Components/DirectionalLight.h"

#include "Real Engine/Components/Transform.h"
#include "Real Engine/Objects/GameObject.h"

DirectionalLight::DirectionalLight(vec3 lightColor) : Light(ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 50.0f), lightColor)
{
}

DirectionalLight::~DirectionalLight()
{
}

LightInfos DirectionalLight::GetLightInfos()
{
	return LightInfos(DIRECTIONAL_LIGHT, GetParent()->transform->Forward(), lightColor, 1, 0, 0, vec3(0.15, 0.15, 0.15), vec3(1, 1, 1), vec3(1, 1, 1), GetViewProjectionMatrix(), GetDepthMapBindlessHandle());
}
