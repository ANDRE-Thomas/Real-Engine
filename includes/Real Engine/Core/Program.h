#pragma once

#include "Real Engine/Rendering/Display/Window.h"

class Program
{
public:
	static Window* window;

	static void Init();
	static void StartLoop();
	static void Stop();
};
