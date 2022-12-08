#include "Real Engine/Display/Window.h"

#include <functional>

#include "Real Engine/Objects/GameObject.h"

Window::Window(WindowOptions options)
{
	this->renderingPipeline = options.renderingPipeline;

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

	glfwSetWindowUserPointer(window, this);

	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height)
		{
			static_cast<Window*>(glfwGetWindowUserPointer(window))->OnResize(width, height);
		});

	// Set Window clear color
	glClearColor(options.baseColor.r, options.baseColor.g, options.baseColor.b, 0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	renderingPipeline->Init();
}

Window::~Window()
{
	glfwTerminate();
}

void Window::Draw(Camera* camera)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	renderingPipeline->Render(camera);

	glfwSwapBuffers(window);
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
	return static_cast<float>(width) / static_cast<float>(height);
}

GLFWwindow* Window::GetGLFWWindow()
{
	return window;
}

bool Window::CloseRequested()
{
	return glfwWindowShouldClose(window) != 0;
}

void Window::OnResize(int width, int height)
{
	this->width = width;
	this->height = height;

	glViewport(0, 0, width, height);

	std::vector<Camera*> cameras = GameObject::FindAllComponentsOfType<Camera>();

	for (size_t i = cameras.size(); i-- > 0;)
		cameras[i]->UpdateProjectionMatrix();
}