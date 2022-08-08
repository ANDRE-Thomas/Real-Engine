#pragma once

#include "Components/Component.h"
#include <vector>
#include <stdexcept>

struct LightInfos
{
	vec4 lightPositionOrDirection;
	vec3 lightColor;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	LightInfos(vec3 lightPositionOrDirection, bool isDirectional, vec3 lightColor, float constant,
		float linear, float quadratic, vec3 ambient, vec3 diffuse, vec3 specular)
	{
		this->lightPositionOrDirection = vec4(lightPositionOrDirection, isDirectional ? 0 : 1);
		this->lightColor = lightColor;
		this->constant = constant;
		this->linear = linear;
		this->quadratic = quadratic;
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
	}
};

class Light : public Component
{
public:
	static std::vector<Light*> lights;

	static std::vector<LightInfos> GetAllLightsInfos();

public:
	vec3 lightColor;

	virtual ~Light();

protected:
	Light(vec3 lightColor);

private:
	virtual LightInfos GetLightInfos() { throw std::runtime_error("GetLightInfos was not implemented"); };
};