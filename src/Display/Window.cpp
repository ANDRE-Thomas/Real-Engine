#include "Display/Window.h"

Window::Window(WindowOptions options)
{
	this->height = options.heigth;
	this->width = options.width;

	glfwWindowHint(GLFW_SAMPLES, options.superSampling);
	glfwWindowHint(GLFW_RESIZABLE, options.resizable ? GL_TRUE : GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(width, height, options.name, options.monitor, NULL);
	if (window == NULL)
		throw DisplayInitException("Failed to create GLFW window.");

	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true;
	if (glewInit() != GLEW_OK)
		throw DisplayInitException("Failed to initialize GLEW.");

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Set Window clear color
	glClearColor(options.baseColor.r, options.baseColor.g, options.baseColor.b, 0);

	// VAO
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	do
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	while (glfwWindowShouldClose(window) == 0);

	glfwTerminate();
}

Window::~Window()
{
	glfwTerminate();
}