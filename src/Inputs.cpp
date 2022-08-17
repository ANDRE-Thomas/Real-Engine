#include "Inputs.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "Program.h"

vec2 Inputs::GetMousePosition()
{
	double x, y;
	glfwGetCursorPos(Program::window->GetGLFWWindow(), &x, &y);

	return vec2(x, y);
}

bool Inputs::GetKeyPressed(int key)
{
	return glfwGetKey(Program::window->GetGLFWWindow(), key) == GLFW_PRESS;
}

bool Inputs::GetKeyReleased(int key)
{
	return glfwGetKey(Program::window->GetGLFWWindow(), key) == GLFW_RELEASE;
}

void Inputs::SetCursorVisibility(bool visible)
{
	glfwSetInputMode(Program::window->GetGLFWWindow(), GLFW_CURSOR, visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}