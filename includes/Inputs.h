#pragma once

#include "glm/glm.hpp"
using namespace glm;

class Inputs
{
public:
	static vec2 GetMousePosition();
	static bool GetKeyPressed(int key);
	static bool GetKeyReleased(int key);

	static void SetCursorVisibility(bool visible);
};