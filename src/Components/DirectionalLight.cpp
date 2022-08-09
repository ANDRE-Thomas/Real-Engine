#include "Components/DirectionalLight.h"

#include "Log.h"
#include "Components/Transform.h"
#include "Objects/GameObject.h"

DirectionalLight* DirectionalLight::instance;

DirectionalLight* DirectionalLight::GetInstance()
{
	return instance;
}

DirectionalLight::DirectionalLight(vec3 lightColor) : Light(lightColor)
{
	if (instance != nullptr)
	{
		Log::Warning("Can't have more than one directional light!");
		return;
	}

	instance = this;
}

DirectionalLight::~DirectionalLight()
{
	if (instance == this)
		instance = nullptr;
}

LightInfos DirectionalLight::GetLightInfos()
{
	return LightInfos(DIRECTIONAL_LIGHT, GetParent()->transform->Forward(), lightColor, 1, 0, 0, vec3(0.15, 0.15, 0.15), vec3(1, 1, 1), vec3(1, 1, 1));
}
