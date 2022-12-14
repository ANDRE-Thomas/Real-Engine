#pragma once

#include "Real Engine/Components/Light.h"

class PointLight : public Light
{
public:
	PointLight(vec3 color);

protected:
	~PointLight();

	virtual LightInfos GetLightInfos();
};