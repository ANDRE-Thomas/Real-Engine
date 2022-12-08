#include "Real Engine/Inputs.h"

std::map<int, Inputs::KeyState> Inputs::keyStates = std::map<int, KeyState>();

GLFWwindow* Inputs::context;

void Inputs::InitInputs(GLFWwindow* context)
{
	Inputs::context = context;

	glfwSetKeyCallback(context, OnKeyUpdate);

	SetCursorVisibility(false);
}

void Inputs::Update()
{
	for (auto it = keyStates.begin(); it != keyStates.end();)
	{
		switch (it->second)
		{
		case Pressed:
			keyStates[it++->first] = Holded;
			break;

		case Released:
			keyStates.erase(it++);
			break;

		case Holded:
			++it;
			break;
		}
	}

	glfwPollEvents();
}

vec2 Inputs::GetMousePosition()
{
	double x, y;
	glfwGetCursorPos(context, &x, &y);

	return { x, y };
}

bool Inputs::GetKeyHold(int key)
{
	auto pair = keyStates.find(key);

	if (pair != keyStates.end())
		return pair->second == Holded || pair->second == Pressed;

	return false;
}

bool Inputs::GetKeyPress(int key)
{
	auto pair = keyStates.find(key);

	if (pair != keyStates.end())
		return pair->second == Pressed;

	return false;
}

bool Inputs::GetKeyRelease(int key)
{
	auto pair = keyStates.find(key);

	if (pair != keyStates.end())
		return pair->second == Released;

	return false;
}

void Inputs::SetCursorVisibility(bool visible)
{
	glfwSetInputMode(context, GLFW_CURSOR, visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}

void Inputs::OnKeyUpdate(GLFWwindow*, int key, int, int action, int)
{
	if (action == GLFW_PRESS)
	{
		auto pair = keyStates.find(key);

		if (pair == keyStates.end())
			keyStates.insert({ key, Pressed});
	}
	else if (action == GLFW_RELEASE)
	{
		auto pair = keyStates.find(key);

		if (pair != keyStates.end())
			keyStates[key] = Released;
	}
}