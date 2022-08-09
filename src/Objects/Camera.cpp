#include "Objects/Camera.h"

#include "Components/Transform.h"

Camera::Camera(float fov, float ratio, float nearPlane, float farPlane) : GameObject()
{
	projectionMatrix = perspective(radians(fov), ratio, nearPlane, farPlane);
}		

Camera::~Camera()
{
}

mat4 Camera::GetRenderMatrix()
{
	return projectionMatrix * inverse(transform->GetMatrix4x4());
}