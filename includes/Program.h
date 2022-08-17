#pragma once

#include "Display/Window.h"
#include "Objects/GameObject.h"

class Program
{
public:
	Window* window;
	GameObject* camera;

	void InitGraph();
	void StartLoop();
	void Stop();
};