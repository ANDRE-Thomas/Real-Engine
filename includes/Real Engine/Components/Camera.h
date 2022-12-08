#pragma once

#include "Real Engine/Rendering/IRenderingViewPoint.h"

class Camera : public IRenderingViewPoint
{
	friend class Window;

public:
	static Camera* GetMainCamera();

private:
	static Camera* mainCamera;

public:
	//Constructor used for creating fullscreen camera
	Camera(float fov, float nearPlane, float farPlane);
	//Constructor used for creating non fullscreen camera
	Camera(float fov, float nearPlane, float farPlane, float ratio);
	~Camera() override;

	void SetAsMainCamera();

private:
	float fov;
	float ratio;
	float nearPlane;
	float farPlane;

	bool isFullscreen;
	bool isMainCamera;

	void UpdateProjectionMatrix();
	mat4 GetProjectionMatrix(float fov, float nearPlane, float farPlane, float ratio, bool isFullscreen);
};