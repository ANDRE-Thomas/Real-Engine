#pragma once

#include "Components/Component.h"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>
using namespace glm;

#include <cmath>

class Transform : public Component
{
public:
	Transform();
	Transform(vec3 position);
	Transform(vec3 position, quat rotation);
	Transform(vec3 position, quat rotation, vec3 scale);
	Transform(mat4 matrix4x4);

	vec3 GetPosition();
	vec3 GetScale();
	quat GetRotation();

	void SetPosition(vec3 position);
	void SetScale(vec3 scale);
	void SetRotation(quat quaternion);

	void Translate(vec3 translation);
	void Scale(vec3 scalingFactor);
	void Rotate(quat rotation);
	void RotateAxis(vec3 axis, float angle);

	mat4 GetMatrix4x4();

private:
	vec3 position;
	quat rotation;
	vec3 scale;
};