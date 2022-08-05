#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
public:
	Window(float height, float width);
	~Window();

private:
	GLFWwindow* window;

	float height, width;

};