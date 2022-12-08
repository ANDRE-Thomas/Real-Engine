#include "Real Engine/Components/Camera.h"

#include "Real Engine/Program.h"
#include "Real Engine/Display/Window.h"

Camera* Camera::mainCamera = nullptr;

Camera* Camera::GetMainCamera()
{
	return mainCamera;
}

Camera::Camera(float fov, float nearPlane, float farPlane) : IRenderingViewPoint(GetProjectionMatrix(fov, nearPlane, farPlane, {}, true)),
fov{ fov },
ratio{},
nearPlane{ nearPlane },
farPlane{ farPlane },
isFullscreen{ true },
isMainCamera{ false }
{
	if (mainCamera == nullptr)
		SetAsMainCamera();
}

Camera::Camera(float fov, float nearPlane, float farPlane, float ratio) : IRenderingViewPoint(GetProjectionMatrix(fov, nearPlane, farPlane, ratio, false)),
fov{ fov },
ratio{ ratio },
nearPlane{ nearPlane },
farPlane{ farPlane },
isFullscreen{ false },
isMainCamera{ false }
{
	if (mainCamera == nullptr)
		SetAsMainCamera();
}

Camera::~Camera()
{
	if (isMainCamera)
		mainCamera = nullptr;
}

void Camera::SetAsMainCamera()
{
	if (mainCamera != nullptr)
		mainCamera->isMainCamera = false;

	mainCamera = this;
}

void Camera::UpdateProjectionMatrix()
{
	projectionMatrix = GetProjectionMatrix(fov, nearPlane, farPlane, ratio, isFullscreen);
}

mat4 Camera::GetProjectionMatrix(float fov, float nearPlane, float farPlane, float ratio, bool isFullscreen)
{
	return perspective(radians(fov), isFullscreen ? Program::window->GetAspectRatio() : ratio, nearPlane, farPlane);
}