#pragma once

#include "Display/Window.h"
#include "Objects/GameObject.h"

class Program
{
public:
	static Window* window;

	static void Init();
	static void StartLoop();
	static void Stop();
};