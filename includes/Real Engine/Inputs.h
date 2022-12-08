#pragma once

#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
using namespace glm;

#include <map>

static class Inputs
{
public:
	static void InitInputs(GLFWwindow* context);

	static void Update();

	static vec2 GetMousePosition();

	static bool GetKeyHold(int key);
	static bool GetKeyPress(int key);
	static bool GetKeyRelease(int key);

	static void SetCursorVisibility(bool visible);

private:
	enum KeyState { Pressed, Holded, Released };

	static std::map<int, KeyState> keyStates;

	static GLFWwindow* context;

	static void OnKeyUpdate(GLFWwindow*, int key, int, int action, int);
};