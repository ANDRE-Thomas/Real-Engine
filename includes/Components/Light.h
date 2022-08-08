#pragma once

#include "glm/glm.hpp"
using namespace glm;

#include "Components/Component.h"

class Light : public Component
{
public:
	vec3 lightColor;

	Light(vec3 lightColor);
	~Light();
};