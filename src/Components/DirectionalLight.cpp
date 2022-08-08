#include "Components/DirectionalLight.h"

#include "Log.h"

DirectionalLight*  DirectionalLight::instance;

DirectionalLight* DirectionalLight::GetInstance()
{
	return instance;
}

DirectionalLight::DirectionalLight(vec3 lightColor) : Light(lightColor)
{
	if (instance != nullptr)
	{
		Log::Warning("Can't have more than one directional light!");
		delete(this);
	}

	instance = this;
}

DirectionalLight::~DirectionalLight()
{
	if (instance == this)
		instance = nullptr;
}
