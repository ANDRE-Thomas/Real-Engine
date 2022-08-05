#include "Program.h"

#include "Objects/GameObject.h"
#include "Rendering/Mesh.h"
#include "Components/Renderer.h"


#include <glm/gtx/string_cast.hpp>

void Program::InitGraph()
{
	if (!glfwInit())
		throw GraphInitException("GLFW Init Error");

	try
	{
		WindowOptions options = WindowOptions("Game", false, false, 1280, 720, 4, vec3(0, 0, 0));
		window = new Window(options);
	}
	catch (const GraphInitException& exception)
	{
		glfwTerminate();
		throw GraphInitException(exception);
	}

	camera = new Camera(60.0f, window->GetAspectRatio(), 0.1f, 100.0f);
	
	GameObject* cube = new GameObject();
	cube->AddComponent(new Renderer(new Mesh("res/models/Cube.obj"), new Material("res/shaders/", "default")));

	GameObject* cube2 = new GameObject();
	cube2->AddComponent(new Renderer(new Mesh("res/models/Cube.obj"), new Material("res/shaders/", "default")));
	cube2->transform->SetPosition(vec3(-3, 0, 0));

	camera->transform->SetPosition(vec3(2, 0,10));
	camera->transform->RotateAxis(vec3(0, 1, 0), 45.0f);

	StartLoop();
}

void Program::StartLoop()
{
	do
	{
		window->Draw(camera);
	} while (!window->CloseRequested());

	glfwTerminate();
}