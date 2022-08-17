#pragma once

#include "Components/Light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight(vec3 lightColor);
	~DirectionalLight();

private:
	virtual LightInfos GetLightInfos();
};