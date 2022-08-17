#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
using namespace glm;

#include "Rendering/RenderingPipeline.h"

struct WindowOptions
{
	const char* name;
	bool fullscreen, resizable;
	int width, heigth;
	int superSampling;

	GLFWmonitor* monitor;
	RenderingPipeline* renderingPipeline;

	vec3 baseColor;

	WindowOptions(const char* name, bool fullscreen, bool resizable, int width, int height, RenderingPipeline* renderingPipeline, int superSampling, vec3 baseColor)
	{
		this->name = name;

		this->fullscreen = fullscreen;
		this->resizable = !fullscreen && resizable;
		monitor = fullscreen ? glfwGetPrimaryMonitor() : NULL;

		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

		this->width = fullscreen ? mode->width : width;
		this->heigth = fullscreen ? mode->height : height;

		this->renderingPipeline = renderingPipeline;

		this->superSampling = superSampling;

		this->baseColor = baseColor;
	}
};