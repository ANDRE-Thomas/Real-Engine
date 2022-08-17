#include "Real Engine/Inputs.h"

std::map<int, Inputs::KeyState> Inputs::keyStates;

GLFWwindow* Inputs::context;

void Inputs::InitInputs(GLFWwindow* context)
{
	Inputs::context = context;

	SetCursorVisibility(false);

	glfwSetKeyCallback(context, OnKeyUpdate);
}

void Inputs::Update()
{
	for (auto it = keyStates.begin(); it != keyStates.end(); ++it)
	{
		if (it->second == Inputs::KeyState::Pressed)
			keyStates[it->first] = Inputs::KeyState::Holded;
		else if (it->second == Inputs::KeyState::Released)
			keyStates.erase(it->first);
	}

	glfwPollEvents();
}

vec2 Inputs::GetMousePosition()
{
	double x, y;
	glfwGetCursorPos(context, &x, &y);

	return vec2(x, y);
}

bool Inputs::GetKeyHold(int key)
{
	auto pair = keyStates.find(key);

	if (pair != keyStates.end())
		return pair->second == Inputs::KeyState::Holded || pair->second == Inputs::KeyState::Pressed;

	return false;
}

bool Inputs::GetKeyPress(int key)
{
	auto pair = keyStates.find(key);

	if (pair != keyStates.end())
		return pair->second == Inputs::KeyState::Pressed;

	return false;
}

bool Inputs::GetKeyRelease(int key)
{
	auto pair = keyStates.find(key);

	if (pair != keyStates.end())
		return pair->second == Inputs::KeyState::Released;

	return false;
}

void Inputs::SetCursorVisibility(bool visible)
{
	glfwSetInputMode(context, GLFW_CURSOR, visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}

void Inputs::OnKeyUpdate(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		auto pair = keyStates.find(key);

		if (pair == keyStates.end())
			keyStates.insert({ key, Inputs::KeyState::Pressed });
	}
	else if (action == GLFW_RELEASE)
	{
		auto pair = keyStates.find(key);

		if (pair != keyStates.end())
			keyStates[key] = Inputs::KeyState::Released;
	}
}