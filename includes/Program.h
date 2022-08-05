#pragma once

#include "Display/Window.h"
#include "Display/WindowOptions.h"
#include "Objects/Camera.h"
#include "Log.h"

class Program
{
public: 
	Window* window;
	Camera* camera;

public:
	void InitGraph();
	void StartLoop();
};