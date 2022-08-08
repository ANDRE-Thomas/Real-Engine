#include "Components/Light.h"

#pragma region Static

std::vector<Light*> Light::lights;

std::vector<LightInfos> Light::GetAllLightsInfos()
{
	std::vector<LightInfos> lightInfos;
	for (size_t i = lights.size(); i-- > 0;)
		lightInfos.push_back(lights[i]->GetLightInfos());

	return lightInfos;
}

#pragma endregion Static

Light::Light(vec3 lightColor) : Component()
{
	this->lightColor = lightColor;

	lights.push_back(this);
}

Light::~Light()
{
	auto result = std::find(lights.begin(), lights.end(), this);
	if (result != lights.end())
		lights.erase(result);
}