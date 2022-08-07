#include "Display/Window.h"

#include "Components/Renderer.h"

Window::Window(WindowOptions options)
{
	this->height = options.heigth;
	this->width = options.width;

	glfwWindowHint(GLFW_SAMPLES, options.superSampling);
	glfwWindowHint(GLFW_RESIZABLE, options.resizable);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(width, height, options.name, options.monitor, NULL);
	if (window == NULL)
		throw GraphInitException("Failed to create GLFW window.");

	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true;
	if (glewInit() != GLEW_OK)
		throw GraphInitException("Failed to initialize GLEW.");

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Set Window clear color
	glClearColor(options.baseColor.r, options.baseColor.g, options.baseColor.b, 0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}

Window::~Window()
{
	glfwTerminate();
}

void Window::Draw(Camera* camera)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Renderer::RenderAll(camera);

	glfwSwapBuffers(window);
	glfwPollEvents();

	deltaTime = glfwGetTime() - lastFrame;
	lastFrame = glfwGetTime();
}

int Window::GetWidth()
{
	return width;
}

int Window::GetHeight()
{
	return height;
}

float Window::GetAspectRatio()
{
	return (float)width / (float)height;
}

float Window::DeltaTime()
{
	return deltaTime;
}

bool Window::CloseRequested()
{
	return glfwWindowShouldClose(window) != 0;
}
