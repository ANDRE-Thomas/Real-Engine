#pragma once

#include "Display/Window.h"
#include "Objects/GameObject.h"

static class Program
{
public:
	static Window* window;
	static GameObject* camera;

	static void Init();
	static void StartLoop();
	static void Stop();
};