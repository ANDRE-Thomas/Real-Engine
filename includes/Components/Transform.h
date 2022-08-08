#pragma once

#include "Components/Component.h"

#include <cmath>

class Transform : public Component
{
public:
	vec3 position;
	quat rotation;
	vec3 scale;

	Transform();
	Transform(vec3 position);
	Transform(vec3 position, quat rotation);
	Transform(vec3 position, quat rotation, vec3 scale);
	Transform(mat4 matrix4x4);

	vec3 Forward();
	vec3 Up();
	vec3 Right();

	void Translate(vec3 translation);
	void Scale(vec3 scalingFactor);
	void Rotate(quat rotation);
	void RotateAxis(vec3 axis, float angle);

	mat4 GetMatrix4x4();
};