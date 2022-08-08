#pragma once

#include "Components/Light.h"

class DirectionalLight : public Light
{
public:
	static DirectionalLight* GetInstance();

private:
	static DirectionalLight* instance;

public:
	DirectionalLight(vec3 lightColor);
	~DirectionalLight();
};