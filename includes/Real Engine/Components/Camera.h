#pragma once

#include "Real Engine/Rendering/IRenderingViewPoint.h"

class Camera : public IRenderingViewPoint
{
public:
	Camera(float fov, float ratio, float nearPlane, float farPlane);
	~Camera();
};