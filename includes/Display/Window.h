#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Display/WindowOptions.h"
#include "Rendering/RenderingPipeline.h"
#include "Components/Camera.h"
#include <stdexcept>

class Window
{
public:
	Window(WindowOptions options);
	~Window();

	void Draw(Camera* camera);

	int GetWidth();
	int GetHeight();
	float GetAspectRatio();
	float DeltaTime();

	GLFWwindow* GetGLFWWindow();
	bool CloseRequested();

private:
	GLFWwindow* window;
	RenderingPipeline* renderingPipeline;

	int height, width;
	double lastFrame = 0;
	float deltaTime = 0;
};

class GraphInitException : public std::runtime_error
{
public:
	GraphInitException(std::string msg) :runtime_error(msg.c_str()) {}
};
