#include "Components/Camera.h"

Camera::Camera(float fov, float ratio, float nearPlane, float farPlane) : IRenderingViewPoint(perspective(radians(fov), ratio, nearPlane, farPlane))
{
}

Camera::~Camera()
{
}