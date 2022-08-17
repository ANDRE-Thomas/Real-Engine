#include "Real Engine/Time.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

double Time::deltaTime, Time::lastFrameTime;

void Time::Update()
{
	deltaTime = float(glfwGetTime() - lastFrameTime);
	lastFrameTime = glfwGetTime();
}

float Time::GetDeltaTime()
{
	return deltaTime;
}