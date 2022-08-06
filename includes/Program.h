#pragma once

#include "Display/Window.h"
#include "Objects/Camera.h"

class Program
{
public: 
	Window* window;
	Camera* camera;

public:
	void InitGraph();
	void StartLoop();
	void Stop();
};