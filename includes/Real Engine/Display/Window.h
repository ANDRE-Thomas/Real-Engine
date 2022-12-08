#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdexcept>

#include "Real Engine/Display/WindowOptions.h"
#include "Real Engine/Rendering/RenderingPipeline.h"
#include "Real Engine/Components/Camera.h"

class Window
{
public:
	Window(WindowOptions options);
	~Window();

	void Draw(Camera* camera);

	int GetWidth();
	int GetHeight();
	float GetAspectRatio();

	GLFWwindow* GetGLFWWindow();
	bool CloseRequested();

private:
	GLFWwindow* window;
	RenderingPipeline* renderingPipeline;

	int height, width;

	void OnResize(int width, int height);
};

class GraphInitException : public std::runtime_error
{
public:
	GraphInitException(std::string msg) :runtime_error(msg.c_str()) {}
};
