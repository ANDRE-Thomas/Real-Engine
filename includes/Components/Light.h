#pragma once

#include "Components/Component.h"
#include <vector>
#include <stdexcept>
#include <cstdlib>

const int DIRECTIONAL_LIGHT = 0;
const int POINT_LIGHT = 1;
const int SPOT_LIGHT = 2;

struct LightInfos
{
	int type;
	alignas(16) vec4 lightPosition;
	alignas(16) vec4 lightColor;

	float constant;
	float linear;
	float quadratic;

	alignas(16) vec4 ambient;
	alignas(16) vec4 diffuse;
	alignas(16) vec4 specular;

	LightInfos(int type, vec3 lightPosition, bool isDirectional, vec3 lightColor, float constant,
		float linear, float quadratic, vec3 ambient, vec3 diffuse, vec3 specular)
	{
		this->type = type;
		this->lightPosition = vec4(lightPosition, 1);
		this->lightColor = vec4(lightColor, 1);
		this->constant = constant;
		this->linear = linear;
		this->quadratic = quadratic;
		this->ambient = vec4(ambient, 1);
		this->diffuse = vec4(diffuse, 1);
		this->specular = vec4(specular, 1);
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