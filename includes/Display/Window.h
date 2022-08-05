#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Display/WindowOptions.h"
#include "Log.h"

class Window
{
public:
	Window(WindowOptions options);
	~Window();

private:
	GLFWwindow* window;

	int height, width;
};

class DisplayInitException : public std::runtime_error
{
public:
	DisplayInitException(std::string msg) :runtime_error(msg.c_str()) {}
};
