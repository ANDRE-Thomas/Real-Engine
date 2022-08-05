#include "Program.h"

void Program::InitWindow()
{
	if (!glfwInit())
		throw DisplayInitException("GLFW Init Error");

	try
	{
		WindowOptions options = WindowOptions("Game", false, false, 1280, 720, 4, vec3(0, 0, 0));
		window = new Window(options);
	}
	catch (const DisplayInitException& exception)
	{
		glfwTerminate();
		throw DisplayInitException(exception);
	}
}