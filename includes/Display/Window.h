#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Display/WindowOptions.h"
#include "Objects/Camera.h"
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

	bool CloseRequested();

private:
	GLFWwindow* window;

	int height, width;
};

class GraphInitException : public std::runtime_error
{
public:
	GraphInitException(std::string msg) :runtime_error(msg.c_str()) {}
};
