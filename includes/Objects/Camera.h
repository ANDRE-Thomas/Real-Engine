#pragma once

#include "Objects/GameObject.h"

class Camera : public GameObject
{
public:
	Camera(float fov, float ratio, float nearPlane, float farPlane);
	~Camera();

public:
	mat4 projectionMatrix;

	mat4 GetRenderMatrix();
};